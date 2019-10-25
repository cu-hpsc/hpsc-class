---
title: "libCEED"
date: 2019-10-25T06:49:25-06:00
toc: true
markup: mmark
type: docs
weight: 2
menu:
  fall2019:
    parent: Lecture Notes
    name: 2019-10-25 libCEED
---

Guest lecture on [libCEED](https://github.com/ceed/libceed) from Dr. [Valeria Barra](https://csel-web.cs.colorado.edu/~vaba3353/): [slides](CUCS_HPSClecture.pdf).

## HPSC Lab 9
2019-10-25

We are going to run some PETSc examples and consider them as "baseline" for the libCEED examples that will follow.


```bash
%%bash

# You may need to change these for your machine
PETSC_DIR=$HOME/petsc-3.12.0 PETSC_ARCH=mpich-dbg

# Build the examples
make -C $PETSC_DIR -f gmakefile $PETSC_ARCH/tests/ksp/ksp/examples/tutorials/ex34
make -C $PETSC_DIR -f gmakefile $PETSC_ARCH/tests/ksp/ksp/examples/tutorials/ex45

# Link them from the current directory to make it easy to run below
cp -sf $PETSC_DIR/$PETSC_ARCH/tests/ksp/ksp/examples/tutorials/ex34 .
cp -sf $PETSC_DIR/$PETSC_ARCH/tests/ksp/ksp/examples/tutorials/ex45 .

./ex34 -pc_type none -da_grid_x 50 -da_grid_y 50 -da_grid_z 50 -ksp_monitor
#run with -ksp_view if you want to see details about the solver [preconditioning]
```

    make: Entering directory '/home/jovyan/petsc-3.12.0'
    make: 'mpich-dbg/tests/ksp/ksp/examples/tutorials/ex34' is up to date.
    make: Leaving directory '/home/jovyan/petsc-3.12.0'
    make: Entering directory '/home/jovyan/petsc-3.12.0'
    make: 'mpich-dbg/tests/ksp/ksp/examples/tutorials/ex45' is up to date.
    make: Leaving directory '/home/jovyan/petsc-3.12.0'
      0 KSP Residual norm 1.184352528131e-01 
      1 KSP Residual norm 4.514009350561e-15 
    Residual norm 4.63793e-15
    Error norm 0.00130921
    Error norm 0.000338459
    Error norm 1.31699e-06



```python
# Another variant with blocked jacobi as smoother:

! ./ex34 -da_grid_x 50 -da_grid_y 50 -da_grid_z 50 -pc_type ksp -ksp_ksp_type cg -ksp_pc_type bjacobi -ksp_monitor 
```

      0 KSP Residual norm 1.251646233668e+02 
      1 KSP Residual norm 1.480869591053e-03 
      2 KSP Residual norm 5.120833590957e-09 
    Residual norm 1.91909e-08
    Error norm 0.00130992
    Error norm 0.000338481
    Error norm 1.31699e-06



```python
# Another variant full multigrid preconditioning

! ./ex34 -pc_type mg -pc_mg_type full -ksp_type fgmres -ksp_monitor_short -pc_mg_levels 3 -mg_coarse_pc_factor_shift_type nonzero
```

      0 KSP Residual norm 1.00731 
      1 KSP Residual norm 0.0510812 
      2 KSP Residual norm 0.00248709 
      3 KSP Residual norm 0.000165921 
      4 KSP Residual norm 1.1586e-05 
      5 KSP Residual norm 8.71845e-07 
    Residual norm 8.71845e-07
    Error norm 0.0208751
    Error norm 0.00618516
    Error norm 0.000197005



```python
# For ex45, compare the number of iterations without precoditioning:

! ./ex45 -pc_type none -da_grid_x 21 -da_grid_y 21 -da_grid_z 21 -ksp_monitor
```

      0 KSP Residual norm 1.470306455035e+01 
      1 KSP Residual norm 2.526523006237e+00 
      2 KSP Residual norm 1.199024543393e+00 
      3 KSP Residual norm 8.017624157084e-01 
      4 KSP Residual norm 5.850738300493e-01 
      5 KSP Residual norm 4.643372450285e-01 
      6 KSP Residual norm 3.794775861442e-01 
      7 KSP Residual norm 3.182229782482e-01 
      8 KSP Residual norm 2.707869730107e-01 
      9 KSP Residual norm 2.342221169435e-01 
     10 KSP Residual norm 2.044268946887e-01 
     11 KSP Residual norm 1.799290014681e-01 
     12 KSP Residual norm 1.597128452355e-01 
     13 KSP Residual norm 1.424463131478e-01 
     14 KSP Residual norm 1.286048456000e-01 
     15 KSP Residual norm 1.180539437186e-01 
     16 KSP Residual norm 1.097826197330e-01 
     17 KSP Residual norm 1.006546027975e-01 
     18 KSP Residual norm 8.528703754785e-02 
     19 KSP Residual norm 6.502594142087e-02 
     20 KSP Residual norm 5.023918850795e-02 
     21 KSP Residual norm 4.014387264317e-02 
     22 KSP Residual norm 2.976949998851e-02 
     23 KSP Residual norm 2.038487027792e-02 
     24 KSP Residual norm 1.483308034344e-02 
     25 KSP Residual norm 1.094830085637e-02 
     26 KSP Residual norm 7.449788171631e-03 
     27 KSP Residual norm 5.269131329764e-03 
     28 KSP Residual norm 3.594369080540e-03 
     29 KSP Residual norm 2.262888004918e-03 
     30 KSP Residual norm 1.493039224295e-03 
     31 KSP Residual norm 1.107124599084e-03 
     32 KSP Residual norm 7.286598548293e-04 
     33 KSP Residual norm 4.716912759260e-04 
     34 KSP Residual norm 3.214892159593e-04 
     35 KSP Residual norm 2.214075669479e-04 
     36 KSP Residual norm 1.645224575275e-04 
     37 KSP Residual norm 1.190806015370e-04 
    Residual norm 0.000119081



```python
# With the ones with preconditioning:

!./ex45 -da_grid_x 21 -da_grid_y 21 -da_grid_z 21 -pc_type mg -pc_mg_levels 3 -mg_levels_ksp_type richardson -mg_levels_ksp_max_it 1 -mg_levels_pc_type bjacobi -ksp_monitor
```

      0 KSP Residual norm 9.713869141172e+01 
      1 KSP Residual norm 1.457128977402e+00 
      2 KSP Residual norm 7.197915243881e-02 
      3 KSP Residual norm 6.946697263348e-04 
    Residual norm 6.67463e-05


## Introduction to libCEED

libCEED is a low-level API library for the efficient
high-order discretization methods developed by the ECP co-design [Center for
Efficient Exascale Discretizations (CEED)](http://ceed.exascaleproject.org).

While our focus is on high-order finite elements, the approach is mostly
algebraic and thus applicable to other discretizations in factored form, as
explained in the API documentation portion of the [Doxygen documentation](https://codedocs.xyz/CEED/libCEED/md_doc_libCEEDapi.html).

Clone or download libCEED by running


```python
! git clone https://github.com/CEED/libCEED.git
```


```python
# then compile it by running

! make -C libCEED -B


```

    make: Entering directory '/home/jovyan/libCEED'
    make: 'lib' with optional backends: /cpu/self/ref/memcheck /cpu/self/avx/serial /cpu/self/avx/blocked
              CC [38;5;177;1mbuild/interface[m/ceed-fortran.o
              CC [38;5;177;1mbuild/interface[m/ceed-basis.o
              CC [38;5;177;1mbuild/interface[m/ceed-elemrestriction.o
              CC [38;5;177;1mbuild/interface[m/ceed-operator.o
              CC [38;5;177;1mbuild/interface[m/ceed-vec.o
              CC [38;5;177;1mbuild/interface[m/ceed.o
              CC [38;5;177;1mbuild/interface[m/ceed-tensor.o
              CC [38;5;177;1mbuild/interface[m/ceed-qfunction.o
              CC [38;5;85;1mbuild/gallery/identity[m/ceed-identity.o
              CC [38;5;93;1mbuild/gallery/poisson3d[m/ceed-poisson3dapply.o
              CC [38;5;93;1mbuild/gallery/poisson3d[m/ceed-poisson3dbuild.o
              CC [38;5;155;1mbuild/gallery/mass1d[m/ceed-massapply.o
              CC [38;5;155;1mbuild/gallery/mass1d[m/ceed-mass1dbuild.o
              CC [38;5;41;1mbuild/gallery/mass2d[m/ceed-mass2dbuild.o
              CC [38;5;47;1mbuild/gallery/poisson1d[m/ceed-poisson1dapply.o
              CC [38;5;47;1mbuild/gallery/poisson1d[m/ceed-poisson1dbuild.o
              CC [38;5;67;1mbuild/gallery/mass3d[m/ceed-mass3dbuild.o
              CC [38;5;211;1mbuild/gallery/poisson2d[m/ceed-poisson2dapply.o
              CC [38;5;211;1mbuild/gallery/poisson2d[m/ceed-poisson2dbuild.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref-basis.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref-operator.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref-qfunction.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref-restriction.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref-tensor.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref-vec.o
              CC [38;5;39;1mbuild/backends/ref[m/ceed-ref.o
              CC [38;5;63;1mbuild/backends/blocked[m/ceed-blocked-operator.o
              CC [38;5;63;1mbuild/backends/blocked[m/ceed-blocked.o
              CC [38;5;93;1mbuild/backends/opt[m/ceed-opt-blocked.o
              CC [38;5;93;1mbuild/backends/opt[m/ceed-opt-operator.o
              CC [38;5;93;1mbuild/backends/opt[m/ceed-opt-serial.o
              CC [38;5;55;1mbuild/backends/memcheck[m/ceed-memcheck-qfunction.o
              CC [38;5;55;1mbuild/backends/memcheck[m/ceed-memcheck.o
              CC [38;5;89;1mbuild/backends/avx[m/ceed-avx-blocked.o
              CC [38;5;89;1mbuild/backends/avx[m/ceed-avx-serial.o
              CC [38;5;89;1mbuild/backends/avx[m/ceed-avx-tensor.o
            LINK [38;5;97;1mlib[m/libceed.so
    make: Leaving directory '/home/jovyan/libCEED'


We are going to look at some libCEED's examples that use some PETSc's capabilities 
(e.g., process partitioning and geometry handling).

Check out my branch for the demo where I made a couple of changes to print more info for the tutorial.


```bash
%%bash
cd libCEED

# checkout my branch for the demo
git checkout valeria/CUHPSC-demo

cd ~/

# And compile the examples by running
make -C libCEED/examples/petsc PETSC_DIR=$HOME/petsc-3.12.0 PETSC_ARCH=mpich-dbg -B

# Link them from the current directory to make it easy to run below
cp -sf libCEED/examples/petsc/bpsraw .
cp -sf libCEED/examples/petsc/multigrid .
```

-----
To run the example solving the Poisson's equation on a structured grid, use


```python
! ./bpsraw -ceed /cpu/self/ref/serial -problem bp3 -degree 1 -local 10000
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 2
        Number of 1D Quadrature Points (q) : 3
        Global nodes                       : 11466
        Process Decomposition              : 1 1 1
        Local Elements                     : 10000 = 20 20 25
        Owned nodes                        : 11466 = 21 21 26
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 2
        Final rnorm                        : 9.710169e-15
      Performance:
        CG Solve Time                      : 0.144075  sec
        DoFs/Sec in CG                     : 0.159167  million
        Pointwise Error (max)              : 2.079708e-02


See what happens when you run this in parallel, let's say with 2 processes


```python
! mpiexec -n 2 ./bpsraw -ceed /cpu/self/ref/serial -problem bp3 -degree 1 -local 10000
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 2
        Number of 1D Quadrature Points (q) : 3
        Global nodes                       : 22386
        Process Decomposition              : 2 1 1
        Local Elements                     : 10000 = 20 20 25
        Owned nodes                        : 10920 = 20 21 26
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 2
        Final rnorm                        : 1.327753e-14
      Performance:
        CG Solve Time                      : 0.229866  sec
        DoFs/Sec in CG                     : 0.194774  million
        Pointwise Error (max)              : 1.999130e-02


Instead, you can keep the total amount of work roughly constant, when you request more processes, 
but divide the local size of the problem so that each process works roughly the same. For instance, compare 


```python
! ./bpsraw -ceed /cpu/self/ref/serial -problem bp3 -degree 1 -local 10000
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 2
        Number of 1D Quadrature Points (q) : 3
        Global nodes                       : 11466
        Process Decomposition              : 1 1 1
        Local Elements                     : 10000 = 20 20 25
        Owned nodes                        : 11466 = 21 21 26
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 2
        Final rnorm                        : 9.710169e-15
      Performance:
        CG Solve Time                      : 0.148991  sec
        DoFs/Sec in CG                     : 0.153915  million
        Pointwise Error (max)              : 2.079708e-02


with 


```python
! mpiexec -n 4 ./bpsraw -ceed /cpu/self/ref/serial -problem bp3 -degree 1 -local 2500
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 2
        Number of 1D Quadrature Points (q) : 3
        Global nodes                       : 11500
        Process Decomposition              : 2 2 1
        Local Elements                     : 2508 = 11 12 19
        Owned nodes                        : 2640 = 11 12 20
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 2
        Final rnorm                        : 9.925692e-15
      Performance:
        CG Solve Time                      : 0.041265  sec
        DoFs/Sec in CG                     : 0.557373  million
        Pointwise Error (max)              : 2.854310e-02


-----

### The multigrid example
This example solves the same problem, but by using a preconditioning strategy. We use Chebchev as the smoother (solver) 
with Jacobi as the preconditioner for the smoother.

Run


```python
! ./multigrid -ceed /cpu/self/ref/serial -problem bp3  -cells 1000
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc + PCMG --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 3
        Number of 1D Quadrature Points (q) : 4
        Global Nodes                       : 49975
        Owned Nodes                        : 49975
      Multigrid:
        Number of Levels                   : 2
        Level 0 (coarse):
          Number of 1D Basis Nodes (p)     : 2
          Global Nodes                     : 3996
          Owned Nodes                      : 3996
        Level 1 (fine):
          Number of 1D Basis Nodes (p)     : 3
          Global Nodes                     : 49975
          Owned Nodes                      : 49975
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 26
        Final rnorm                        : 3.394703e-12
      PCMG:
        PCMG Type                          : MULTIPLICATIVE
        PCMG Cycle Type                    : v
      Performance:
        Pointwise Error (max)              : 4.270543e-02
        CG Solve Time                      : 19.3821 sec
        DoFs/Sec in CG                     : 0.0670388 million


and 


```python
! mpiexec -n 4 ./multigrid -ceed /cpu/self/ref/serial -problem bp3  -cells 1000
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc + PCMG --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 3
        Number of 1D Quadrature Points (q) : 4
        Global Nodes                       : 49975
        Owned Nodes                        : 6995
      Multigrid:
        Number of Levels                   : 2
        Level 0 (coarse):
          Number of 1D Basis Nodes (p)     : 2
          Global Nodes                     : 3996
          Owned Nodes                      : 0
        Level 1 (fine):
          Number of 1D Basis Nodes (p)     : 3
          Global Nodes                     : 49975
          Owned Nodes                      : 6995
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 26
        Final rnorm                        : 3.387644e-12
      PCMG:
        PCMG Type                          : MULTIPLICATIVE
        PCMG Cycle Type                    : v
      Performance:
        Pointwise Error (max)              : 4.270543e-02
        CG Solve Time                      : 33.2205 sec
        DoFs/Sec in CG                     : 0.0391129 million


What do you see? 

Now let's raise the degree (accuracy of solution). 
This will also increase the number of neighboring points we need information from, i.e., the number of nodes.

Compare


```python
! ./multigrid -ceed /cpu/self/ref/serial -problem bp3 -degree 8 
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc + PCMG --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 9
        Number of 1D Quadrature Points (q) : 10
        Global Nodes                       : 12167
        Owned Nodes                        : 12167
      Multigrid:
        Number of Levels                   : 8
        Level 0 (coarse):
          Number of 1D Basis Nodes (p)     : 2
          Global Nodes                     : 8
          Owned Nodes                      : 8
        Level 7 (fine):
          Number of 1D Basis Nodes (p)     : 9
          Global Nodes                     : 12167
          Owned Nodes                      : 12167
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 6
        Final rnorm                        : 3.132892e-11
      PCMG:
        PCMG Type                          : MULTIPLICATIVE
        PCMG Cycle Type                    : v
      Performance:
        Pointwise Error (max)              : 4.525195e-08
        CG Solve Time                      : 1.60019 sec
        DoFs/Sec in CG                     : 0.0456207 million


With


```python
! ./multigrid -ceed /cpu/self/ref/serial -problem bp3 -degree 8 -coarsen logarithmic
```

    
    -- CEED Benchmark Problem 3 -- libCEED + PETSc + PCMG --
      libCEED:
        libCEED Backend                    : /cpu/self/ref/serial
      Mesh:
        Number of 1D Basis Nodes (p)       : 9
        Number of 1D Quadrature Points (q) : 10
        Global Nodes                       : 12167
        Owned Nodes                        : 12167
      Multigrid:
        Number of Levels                   : 4
        Level 0 (coarse):
          Number of 1D Basis Nodes (p)     : 2
          Global Nodes                     : 8
          Owned Nodes                      : 8
        Level 3 (fine):
          Number of 1D Basis Nodes (p)     : 9
          Global Nodes                     : 12167
          Owned Nodes                      : 12167
      KSP:
        KSP Type                           : cg
        KSP Convergence                    : CONVERGED_RTOL
        Total KSP Iterations               : 7
        Final rnorm                        : 1.970523e-11
      PCMG:
        PCMG Type                          : MULTIPLICATIVE
        PCMG Cycle Type                    : v
      Performance:
        Pointwise Error (max)              : 4.525156e-08
        CG Solve Time                      : 0.912013 sec
        DoFs/Sec in CG                     : 0.0933857 million


Without specifying a coarsening strategy, it defaults to `-coarsen uniform`. 
This way, the domain is partitioned from finest grid to coarsest grid in a linear fashion, i.e.,
for `-degree 8`, we run all intermediate levels given by

8->7->6->5->...->2->1

Instead, when we use `-coarsen logarithmic` we have fewer subdivisions, using only powers of 2 as intermediate levels

8->4->2->1

-----

Collect your experiments data and try to plot the accuracy gained 
(given by the error, when the actual solution is available, otherwise by the digits of precision gained) vs time to solve


```python

```
