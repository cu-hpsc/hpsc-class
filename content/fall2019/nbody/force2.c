#include <math.h>

void force2(int m, const float xmass[], const float x[],
            int n, const float ymass[], const float y[],
            float f[]) {
  for (int i=0; i<m; i++) {
    for (int j=0; j<n; j++) {
      float r[3] = {y[0*m+j] - x[0*n+i],
                    y[1*m+j] - x[1*n+i],
                    y[2*m+j] - x[2*n+i]};
      float r2 = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
      float Gmm = xmass[i] * ymass[j] / (r2 * sqrt(r2));
      for (int k=0; k<3; k++)
        f[k*m+i] += Gmm * r[k];
    }
  }
}
