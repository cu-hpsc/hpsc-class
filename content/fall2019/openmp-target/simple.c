#include <stdio.h>
#include <math.h>

void simple_ispc(double vin[], double vout[], int count);

void simple_c(double vin[], double vout[], int count) {
  for (int index=0; index<count; index++) {
    double v = vin[index];
    if (v < 3.)
      v = v * v;
    else
      v = sqrt(v);
    vout[index] = v;
  }
}

int main() {
  double vin[16], vout[16];
  for (int i = 0; i < 16; ++i)
    vin[i] = i;

  simple_ispc(vin, vout, 16);

  for (int i = 0; i < 16; ++i)
    printf("%d: simple_ispc(%f) = %f\n", i, vin[i], vout[i]);

  simple_c(vin, vout, 16);

  for (int i = 0; i < 16; ++i)
    printf("%d: simple_c(%f) = %f\n", i, vin[i], vout[i]);
  return 0;
}
