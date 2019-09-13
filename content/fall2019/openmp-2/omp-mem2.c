#include <omp.h>
#include <stdio.h>

int main() {
  int b=1, c=2;
  #pragma omp parallel firstprivate(b) shared(c)
  {
    int a = 1;
    int id = omp_get_thread_num();
    b++;
    #pragma omp critical
    c++;
    for (int b=0; b<id; b++) {
#pragma omp barrier
      printf("[%d] %d %d %d\n", id, a, b, c);
    }
  }
  printf("END: _ %d %d\n", b, c);

  return 0;
}
