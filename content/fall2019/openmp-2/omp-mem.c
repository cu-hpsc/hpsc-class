#include <omp.h>
#include <stdio.h>

int main() {
  int a=0, b=1, c=2;
  #pragma omp parallel firstprivate(a) firstprivate(b) shared(c)
  {
    a++;
    b++;
    c++;
    int id = omp_get_thread_num();
    printf("[%d] %d %d %d\n", id, a, b, c);
  }
  printf("END: %d %d %d\n", a, b, c);
  return 0;
}
