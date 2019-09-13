#include "rdtsc.h"

#include <argp.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Args {
  size_t length;
  size_t nreps;
  bool block;
};

static struct argp_option options[] = {
  {"length", 'n', "size_t", 0, "Length of each vector"},
  {"nreps", 'r', "size_t", 0, "Number of repetitions"},
  {"block", 'b', NULL, 0, "Compute block dot products (versus a single dot product)"},
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct Args *args = state->input;
  switch (key) {
  case ARGP_KEY_INIT:
    args->length = 100;
    args->nreps = 10;
    args->block = false;
    break;
  case 'n':
    args->length = strtol(arg, NULL, 10);
    break;
  case 'r':
    args->nreps = strtol(arg, NULL, 10);
    break;
  case 'b':
    args->block = true;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

__attribute((noinline))
static double dot_ref(size_t n, const double *a, const double *b) {
  double sum = 0;
  for (size_t i=0; i<n; i++)
    sum += a[i] * b[i];
  return sum;
}

#define BS 16

__attribute((noinline))
static double dot_opt1(size_t n, const double *a, const double *b) {
  double sums[4] = {};
  omp_set_num_threads(4);
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    for (size_t i=id; i<n; i+=4)
      sums[id] += a[i] * b[i];
  }
  for (size_t j=1; j<4; j++) sums[0] += sums[j];
  return sums[0];
}

__attribute((noinline))
static double dot_opt2(size_t n, const double *a, const double *b) {
  double sums[4] = {};
  omp_set_num_threads(4);
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    #pragma omp for
    for (size_t i=0; i<n; i++)
      sums[id] += a[i] * b[i];
  }
  for (size_t j=1; j<4; j++) sums[0] += sums[j];
  return sums[0];
}

__attribute((noinline))
static double dot_opt3(size_t n, const double *a, const double *b) {
  double sum = 0;
  omp_set_num_threads(4);
  #pragma omp parallel
  {
    #pragma omp for reduction(+:sum)
    for (size_t i=0; i<n; i++)
      sum += a[i] * b[i];
  }
  return sum;
}

static void report_dot(const char *name, ticks_t start_ticks, size_t flops, double result) {
  ticks_t ticks = rdtsc() - start_ticks;
  double rate = 1.*flops / ticks;
  if (fabs(result - flops) > 1e-10)
    printf("Result %f failed to validate with expected value %ld\n", result, flops);
  printf("%8s\t%ld\t%lld\t%8.2f\t\n", name, flops, ticks, rate);
}

#define REPORT_DOT(f) do {                                              \
    for (int rep=0; rep<args.nreps; rep++) {                            \
      ticks_t ticks_start = rdtsc();                                    \
      report_dot(#f, ticks_start, 2*args.length, f(args.length, a, b)); \
    }                                                                   \
  } while (0)

// Dimensions of the matrices for block dot products.
#define J 8
#define K 4

// Performs the operation
//   C = A * B
// where A and B have shape (J,n) and (n,K) respectively.
// This reference version stores A as row-major and B as column-major.
static void bdot_ref(size_t n, const double *a, const double *b, double *c) {
  for (size_t j=0; j<J; j++) {
    for (size_t k=0; k<K; k++) {
      c[j*K+k] = dot_ref(n, &a[j*n], &b[k*n]);
    }
  }
}

static void bdot_opt(size_t n, const double *a, const double *b, double *c) {
  bdot_ref(n, a, b, c);
}

static void init_bdot(size_t n, double *a, size_t ajstride, size_t aistride,
                      double *b, size_t bistride, size_t bkstride) {
  for (size_t i=0; i<n; i++) {
    for (size_t j=0; j<J; j++)
      a[i*aistride + j*ajstride] = 1000*(i+1) + j+1;
    for (size_t k=0; k<K; k++)
      b[i*bistride + k*bkstride] = 1./(1000*(i+1) + k+1);
  }
}

static void report_bdot(const char *name, ticks_t start_ticks, size_t flops,
                        const double *result, int jstride, int kstride,
                        const double *ref_result) {
  ticks_t ticks = rdtsc() - start_ticks;
  double rate = 1.*flops / ticks;
  if (result && ref_result && result != ref_result) {
    for (int j=0; j<J; j++) {
      for (int k=0; k<K; k++) {
        if (fabs(result[j*jstride + k*kstride] - ref_result[j*K+k]) > 1e-10) {
          printf("Result[%d,%d] = %f failed to validate with expected value %f\n", j, k, result[j*jstride + k*kstride], ref_result[j*K+k]);
        }
      }
    }
  }
  printf("%s\t%ld\t%lld\t%8.2f\t\n", name, flops, ticks, rate);
}

#define REPORT_BDOT(f, c, jstride, kstride, c_ref) do {                 \
    for (int rep=0; rep<args.nreps; rep++) {                            \
      ticks_t ticks_start = rdtsc();                                    \
      f(args.length, a, b, c);                                          \
      report_bdot(#f, ticks_start, 2*J*K*args.length, c, jstride, kstride, c_ref); \
    }                                                                   \
  } while (0)

int main(int argc, char **argv) {
  struct Args args;
  struct argp argp = {options, parse_opt, NULL, NULL};
  argp_parse(&argp, argc, argv, 0, 0, &args);
  size_t n = args.length;

  switch (args.block) {
  case false: {
    double *a = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    for (size_t i=0; i<n; i++) {
      a[i] = 2.*(i+1);
      b[i] = 1./(i+1);
    }

    printf("  Name  \tflops\tticks\tflops/tick\n");
    REPORT_DOT(dot_ref);
    REPORT_DOT(dot_opt1);
    REPORT_DOT(dot_opt2);
    REPORT_DOT(dot_opt3);

    free(a); free(b);
  } break;
  case true: {
    double *a = malloc(J * n * sizeof(double));
    double *b = malloc(K * n * sizeof(double));
    double *c = malloc(J * K * sizeof(double));
    double *c_ref = malloc(J * K * sizeof(double));

    printf("Name    \tflops\tticks\tflops/tick\n");
    init_bdot(args.length, a, n, 1, b, 1, n);
    REPORT_BDOT(bdot_ref, c_ref, K, 1, c_ref);

    // You may initialize a and b differently and call more variants by editing
    // the two lines below, or by creating new variants.
    init_bdot(args.length, a, n, 1, b, 1, n);
    REPORT_BDOT(bdot_opt, c, K, 1, c_ref);

    free(a); free(b); free(c); free(c_ref);
  } break;
  }
  return 0;
}
