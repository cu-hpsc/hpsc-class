#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s N\n", argv[0]);
    return 1;
  }
  long N = atol(argv[1]);
  long fibs[N];
  fibs[0] = 1;
  fibs[1] = 2;
  for (long i=2; i<N; i++)
    fibs[i] = fibs[i-1] + fibs[i-2];
  for (long i=0; i<N; i++)
    printf("%2ld: %5ld\n", i+1, fibs[i]);
  return 0;
}
