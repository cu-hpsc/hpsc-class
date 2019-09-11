---
title: "OpenMP Basics"
date: 2019-09-11T06:49:25-06:00
toc: true
type: docs
weight: 2
menu:
  fall2019:
    parent: Lecture Notes
    name: 2019-09-11 OpenMP Basics
---

```python
def render_c(filename):
    from IPython.display import Markdown
    with open(filename) as f:
        contents = f.read()
    return Markdown("```c\n" + contents + "```\n")
```

## What is [OpenMP](https://en.wikipedia.org/wiki/OpenMP)?

![By Wikipedia user A1 - w:en:File:Fork_join.svg, CC BY 3.0, https://commons.wikimedia.org/w/index.php?curid=32004077](https://upload.wikimedia.org/wikipedia/commons/f/f1/Fork_join.svg)

A community-developed standard Application Programming Interface (with "directives") for
* multithreaded programming
* vectorization
* offload to coprocessors (such as GPUs)

OpenMP is available for C, C++, and Fortran.

Latest version: OpenMP-5.0, released November 2018.  Implementations are still incomplete!

### OpenMP Resources
* [OpenMP-5.0 Reference Cards](https://www.openmp.org/resources/refguides/) (a few pages, printable)
* [OpenMP-5.0 Standard](https://www.openmp.org/spec-html/5.0/openmp.html)
* [OpenMP-4.5 Examples](http://www.openmp.org/wp-content/uploads/openmp-examples-4.5.0.pdf)
* [LLNL Tutorial](https://computing.llnl.gov/tutorials/openMP/)
* [Mattson: The OpenMP Common Core](https://press3.mcs.anl.gov//atpesc/files/2019/07/ATPESC_2019_Track-2_2_7-31_830am_Mattson-The-OpenMP_Common_Core.pdf) from [ATPESC](https://extremecomputingtraining.anl.gov/) ([video](https://www.youtube.com/watch?v=T0csnAirv-U&list=PLGj2a3KTwhRa6Ux64xg5L5ga6Jg8QykoQ&index=2))

### `#pragma omp parallel`

The standard is **big**, but most applications only use a few constructs.


```python
render_c('omp-hello.c')
```




```c
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
```





```python
!make CFLAGS='-fopenmp -Wall' -B omp-hello
```

    cc -fopenmp -Wall    omp-hello.c   -o omp-hello



```python
!./omp-hello
```

    I am 1 of 4
    I am 2 of 4
    I am 0 of 4
    I am 3 of 4



```python
!OMP_NUM_THREADS=8 ./omp-hello
```

    I am 0 of 8
    I am 7 of 8
    I am 1 of 8
    I am 3 of 8
    I am 4 of 8
    I am 6 of 8
    I am 2 of 8
    I am 5 of 8


### Parallelizing `triad`
```c
void triad(int N, double *a, const double *b, double scalar, const double *c) {
#pragma omp parallel
    {
        for (int i=0; i<N; i++)
            a[i] = b[i] + scalar * c[i];
    }
}
```

What does this code do?

```c
void triad(int N, double *a, const double *b, double scalar, const double *c) {
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        for (int i=id; i<N; i+=num_threads)
            a[i] = b[i] + scalar * c[i];
    }
}
```

### Parallelizing `dot`

```c
static double dot_ref(size_t n, const double *a, const double *b) {
  double sum = 0;
  for (size_t i=0; i<n; i++)
    sum += a[i] * b[i];
  return sum;
}
```


```python
!make CFLAGS='-O3 -march=native -fopenmp' -B dot
```

    cc -O3 -march=native -fopenmp    dot.c   -o dot



```python
!OMP_NUM_THREADS=2 ./dot -r 10 -n 10000
```

      Name  	flops	ticks	flops/tick
     dot_ref	20000	40327	    0.50	
     dot_ref	20000	35717	    0.56	
     dot_ref	20000	36096	    0.55	
     dot_ref	20000	36487	    0.55	
     dot_ref	20000	37157	    0.54	
     dot_ref	20000	36024	    0.56	
     dot_ref	20000	35322	    0.57	
     dot_ref	20000	36601	    0.55	
     dot_ref	20000	72193	    0.28	
     dot_ref	20000	37924	    0.53	
    dot_opt1	20000	51256384	    0.00	
    dot_opt1	20000	23343145	    0.00	
    dot_opt1	20000	4646174	    0.00	
    dot_opt1	20000	16710	    1.20	
    dot_opt1	20000	15512	    1.29	
    dot_opt1	20000	16016	    1.25	
    dot_opt1	20000	16982	    1.18	
    dot_opt1	20000	452064	    0.04	
    dot_opt1	20000	16278	    1.23	
    dot_opt1	20000	16311	    1.23	
    dot_opt2	20000	24616	    0.81	
    dot_opt2	20000	16095	    1.24	
    dot_opt2	20000	17561	    1.14	
    dot_opt2	20000	16270	    1.23	
    dot_opt2	20000	18130	    1.10	
    dot_opt2	20000	16831	    1.19	
    dot_opt2	20000	16968	    1.18	
    dot_opt2	20000	16391	    1.22	
    dot_opt2	20000	17063	    1.17	
    dot_opt2	20000	16315	    1.23	
    dot_opt3	20000	77013	    0.26	
    dot_opt3	20000	12419	    1.61	
    dot_opt3	20000	12124	    1.65	
    dot_opt3	20000	12193	    1.64	
    dot_opt3	20000	12051	    1.66	
    dot_opt3	20000	12009	    1.67	
    dot_opt3	20000	11944	    1.67	
    dot_opt3	20000	12032	    1.66	
    dot_opt3	20000	12687	    1.58	
    dot_opt3	20000	12188	    1.64	


### Vectorization

OpenMP-4.0 added the `omp simd` construct, which is a portable way to request that the compiler vectorize code.
An example of a reason why a compiler might fail to vectorize code is aliasing, which we investigate below.


```python
render_c('triad.c')
```




```c
#include <stdlib.h>

void triad(size_t N, double *a, const double *b, double scalar, const double *c) {
  for (size_t i=0; i<N; i++)
    a[i] = b[i] + scalar * c[i];
}
```





```python
!gcc -O2 -ftree-vectorize -fopt-info-all -c triad.c
```

    Unit growth for small function inlining: 15->15 (0%)
    
    Inlined 0 calls, eliminated 0 functions
    
    triad.c:4:3: optimized: loop vectorized using 16 byte vectors
    triad.c:4:3: optimized:  loop versioned for vectorization because of possible aliasing
    triad.c:3:6: note: vectorized 1 loops in function.
    triad.c:4:3: optimized: loop turned into non-loop; it never loops


* gcc autovectorization starts at `-O3` or if you use `-ftree-vectorize`
* options such as [-fopt-info](https://gcc.gnu.org/onlinedocs/gcc/Developer-Options.html#index-fopt-info) give useful diagnostics, but are compiler-dependent and sometimes referring to assembly is useful
* `man gcc` with search (`/`) is your friend

### What is aliasing?

Is this valid code?  What xs `x` after this call?
```c
double x[5] = {1, 2, 3, 4, 5};
triad(2, x+1, x, 10., x);
```

C allows memory to overlap arbitrarily.  You can inform the compiler of this using the [`restrict` qualifier](https://en.wikipedia.org/wiki/Restrict) (C99/C11; `__restrict` or `__restrict__` work with most C++ and [CUDA](https://devblogs.nvidia.com/cuda-pro-tip-optimize-pointer-aliasing/) compilers).


```python
render_c('triad-restrict.c')
```




```c
void triad(int N, double *restrict a, const double *restrict b, double scalar, const double *restrict c) {
  for (int i=0; i<N; i++)
    a[i] = b[i] + scalar * c[i];
}
```





```python
!gcc -O2 -march=native -ftree-vectorize -fopt-info-all -c triad-restrict.c
```

    Unit growth for small function inlining: 15->15 (0%)
    
    Inlined 0 calls, eliminated 0 functions
    
    triad-restrict.c:2:5: optimized: loop vectorized using 32 byte vectors
    triad-restrict.c:1:6: note: vectorized 1 loops in function.


Notice how there is no more `loop versioned for vectorization because of possible aliasing`.

The complexity of checking for aliasing can grow combinatorially in the number of arrays being processed, leading to many loop variants and/or preventing vectorization.

#### Aside: Warnings
The `-Wrestrict` flag (included in `-Wall`) can catch some programming errors
```c
void foo(double *x) {
  triad(2, x, x, 10, x);
}
```


```python
!gcc -O2 -Wall -c triad-foo.c
```

The powers of `-Wrestrict` are limited, however, and (as of gcc-9) do not even catch
```c
void foo(double *x) {
  triad(2, x+1, x, 10, x);
}
```

### Check the assembly


```python
!objdump -d --prefix-addresses -M intel triad-restrict.o
```

    
    triad-restrict.o:     file format elf64-x86-64
    
    
    Disassembly of section .text:
    0000000000000000 <triad> test   edi,edi
    0000000000000002 <triad+0x2> jle    0000000000000067 <triad+0x67>
    0000000000000004 <triad+0x4> lea    eax,[rdi-0x1]
    0000000000000007 <triad+0x7> cmp    eax,0x2
    000000000000000a <triad+0xa> jbe    0000000000000074 <triad+0x74>
    000000000000000c <triad+0xc> mov    r8d,edi
    000000000000000f <triad+0xf> shr    r8d,0x2
    0000000000000013 <triad+0x13> vbroadcastsd ymm2,xmm0
    0000000000000018 <triad+0x18> shl    r8,0x5
    000000000000001c <triad+0x1c> xor    eax,eax
    000000000000001e <triad+0x1e> xchg   ax,ax
    0000000000000020 <triad+0x20> vmovupd ymm1,YMMWORD PTR [rcx+rax*1]
    0000000000000025 <triad+0x25> vfmadd213pd ymm1,ymm2,YMMWORD PTR [rdx+rax*1]
    000000000000002b <triad+0x2b> vmovupd YMMWORD PTR [rsi+rax*1],ymm1
    0000000000000030 <triad+0x30> add    rax,0x20
    0000000000000034 <triad+0x34> cmp    rax,r8
    0000000000000037 <triad+0x37> jne    0000000000000020 <triad+0x20>
    0000000000000039 <triad+0x39> mov    eax,edi
    000000000000003b <triad+0x3b> and    eax,0xfffffffc
    000000000000003e <triad+0x3e> test   dil,0x3
    0000000000000042 <triad+0x42> je     0000000000000070 <triad+0x70>
    0000000000000044 <triad+0x44> vzeroupper 
    0000000000000047 <triad+0x47> cdqe   
    0000000000000049 <triad+0x49> nop    DWORD PTR [rax+0x0]
    0000000000000050 <triad+0x50> vmovsd xmm1,QWORD PTR [rcx+rax*8]
    0000000000000055 <triad+0x55> vfmadd213sd xmm1,xmm0,QWORD PTR [rdx+rax*8]
    000000000000005b <triad+0x5b> vmovsd QWORD PTR [rsi+rax*8],xmm1
    0000000000000060 <triad+0x60> inc    rax
    0000000000000063 <triad+0x63> cmp    edi,eax
    0000000000000065 <triad+0x65> jg     0000000000000050 <triad+0x50>
    0000000000000067 <triad+0x67> ret    
    0000000000000068 <triad+0x68> nop    DWORD PTR [rax+rax*1+0x0]
    0000000000000070 <triad+0x70> vzeroupper 
    0000000000000073 <triad+0x73> ret    
    0000000000000074 <triad+0x74> xor    eax,eax
    0000000000000076 <triad+0x76> jmp    0000000000000047 <triad+0x47>


* How do the results change if you go up and replace `-march=native` with `-march=skylake-avx512 -mprefer-vector-width=512`?
* Is the assembly qualitatively different without `restrict` (in which case the compiler "versions" the loop).

### Pragma `omp simd`

An alternative (or supplement) to `restrict` is `#pragma omp simd`.


```python
render_c('triad-omp-simd.c')
```




```c
void triad(int N, double *a, const double *b, double scalar, const double *c) {
#pragma omp simd
  for (int i=0; i<N; i++)
    a[i] = b[i] + scalar * c[i];
}
```





```python
!gcc -O2 -march=native -ftree-vectorize -fopenmp -fopt-info-all -c triad-omp-simd.c
```

    Unit growth for small function inlining: 15->15 (0%)
    
    Inlined 0 calls, eliminated 0 functions
    
    triad-omp-simd.c:4:17: optimized: loop vectorized using 32 byte vectors
    triad-omp-simd.c:1:6: note: vectorized 1 loops in function.

