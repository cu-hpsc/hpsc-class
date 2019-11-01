__global__ void copy(float *dst, float *src) {
  int iblock = blockIdx.x + blockIdx.y * gridDim.x;
  int index  = threadIdx.x + TILE_SIZE * iblock * blockDim.x;
  float a[TILE_SIZE]; // allocated in registers
  for (int i=0; i<TILE_SIZE; i++)
    a[i] = src[index + i * blockDim.x];
  for (int i=0; i<TILE_SIZE; i++)
    dst[index + i * blockDim.x] = a[i];
}
