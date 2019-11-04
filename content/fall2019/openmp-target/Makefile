ISPC = ispc
ISPCFLAGS = -O3 --target=avx2-i32x8
CFLAGS = -O3 -march=native

%.o : %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o $@

simple : LDLIBS = -lm
simple : simple.o simple-ispc.o
