void triad(int N, double *a, const double *b, double scalar, const double *c) {
#pragma omp simd
    for (int i=0; i<N; i++)
        a[i] = b[i] + scalar * c[i];
}
