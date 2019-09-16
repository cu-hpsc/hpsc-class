#include <stdio.h>
#include <stdlib.h>

long fib(long n) {
  if (n < 2) return n;
  return fib(n - 1) + fib(n - 2);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s N\n", argv[0]);
    return 1;
  }
  long N = atol(argv[1]);
  long fibs[N];
  #pragma omp parallel for schedule(static,1)
  for (long i=0; i<N; i++)
    fibs[i] = fib(i+1);
  for (long i=0; i<N; i++)
    printf("%2ld: %5ld\n", i+1, fibs[i]);
  return 0;
}
