export void simple_ispc(uniform double vin[], uniform double vout[],
                        uniform int count) {
  foreach (index = 0 ... count) {
    double v = vin[index];
    if (v < 3.)
      v = v * v;
    else
      v = sqrt(v);
    vout[index] = v;
  }
}
