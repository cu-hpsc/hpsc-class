#include <omp.h>
#include <stdio.h>

int main() {
  #pragma omp parallel
  {
    int num_threads = omp_get_num_threads();
    int my_thread_num = omp_get_thread_num();
    printf("I am %d of %d\n", my_thread_num, num_threads);
  }
  return 0;
}
