#include <math.h>

void force1(int m, const float xmass[], const float x[],
            int n, const float ymass[], const float y[],
            float f[]) {
  for (int i=0; i<m; i++) {
    for (int j=0; j<n; j++) {
      float r[3] = {y[j*3+0] - x[i*3+0],
                    y[j*3+1] - x[i*3+1],
                    y[j*3+2] - x[i*3+2]};
      float r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
      float Gmm = xmass[i] * ymass[j] / (r2 * sqrt(r2));
      for (int k=0; k<3; k++)
        f[i*3+k] += Gmm * r[k];
    }
  }
}
