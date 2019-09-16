#include <stdio.h>
#include <stdlib.h>

long fib_seq(long n) {
  if (n < 2) return n;
  return fib_seq(n - 1) + fib_seq(n - 2);
}

long fib(long n) {
  if (n < 30)
    return fib_seq(n);
  long n1, n2;
  #pragma omp task shared(n1)
  n1 = fib(n - 1);
  #pragma omp task shared(n2)
  n2 = fib(n - 2);
  #pragma omp taskwait
  return n1 + n2;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s N\n", argv[0]);
    return 1;
  }
  long N = atol(argv[1]);
  long fibs[N];
  #pragma omp parallel
  #pragma omp single nowait
  {
    for (long i=0; i<N; i++)
      fibs[i] = fib(i+1);
  }
  for (long i=0; i<N; i++)
    printf("%2ld: %5ld\n", i+1, fibs[i]);
  return 0;
}
