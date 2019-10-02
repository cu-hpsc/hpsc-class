#include "rdtsc.h"

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

/* This structure is used by main to communicate with parse_opt. */
struct args
{
  size_t array_len;
  size_t repetitions;
  size_t trials;
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
{
  {"array_len",'a',"size_t",0,"array length"},
  {"repetitions",'r',"size_t",0,"number of repetitions"},
  {"trials",'t',"size_t",0,"number of trials"},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct args *args = state->input;
  switch (key)
    {
    case ARGP_KEY_INIT:
      args->array_len = 15000;
      args->repetitions = 100;
      args->trials = 10;
      break;
    case 'a':
      args->array_len = strtol(arg, NULL, 10);
      break;
    case 'r':
      args->repetitions = strtol(arg, NULL, 10);
      break;
    case 't':
      args->trials = strtol(arg, NULL, 10);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

void parse_args(struct args *args, int argc, char **argv)
{
  static struct argp argp = {options, parse_opt, NULL, NULL};
  argp_parse(&argp, argc, argv, 0, 0, args);
  return;
}


int main(int argc, char **argv)
{
  struct args args;

  // argument parsing via parse.h
  parse_args(&args, argc, argv);

  // initialize array
  double *x = malloc(args.array_len * sizeof(double));
  double *y = malloc(args.array_len * sizeof(double));

  for (int trials=0; trials<args.trials; trials++) {
    for (size_t i=0; i<args.array_len; i++) {
      x[i] = 1.0;
      y[i] = 0.0;
    }

    ticks_t starttick = rdtsc();
#pragma omp parallel
    {
      for (size_t rep=0; rep<args.repetitions; rep++) {
#pragma omp for nowait
        for (size_t i=0; i<args.array_len; i++)
          y[i] += 3.14 * x[i];
      }
    }
    printf("omp for         : %f ticks per entry\n", (double)(rdtsc() - starttick) / (args.array_len * args.repetitions));

    for (size_t i=0; i<args.array_len; i++) {
      x[i] = 1.0;
      y[i] = 0.0;
    }

    starttick = rdtsc();
    for (size_t rep=0; rep<args.repetitions; rep++) {
#pragma omp parallel for
      for (size_t i=0; i<args.array_len; i++)
        y[i] += 3.14 * x[i];
    }
    printf("omp parallel for: %f ticks per entry\n", (double)(rdtsc() - starttick) / (args.array_len * args.repetitions));
  }

  free(x);
  free(y);
  return 0;
}
