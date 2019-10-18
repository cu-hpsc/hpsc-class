---
title: "Multilevel Preconditioning"
date: 2019-10-18T06:49:25-06:00
toc: true
markup: mmark
type: docs
weight: 2
menu:
  fall2019:
    parent: Lecture Notes
    name: 2019-10-18 Multilevel Preconditioning
---

## Recap: Domain Decomposition convergence theory

The formal convergence is beyond the scope of this course, but the following estimates are useful.  We let $h$ be the element diameter, $H$ be the subdomain diameter, and $\delta$ be the overlap, each normalized such that the global domain diameter is 1.  We express the convergence in terms of the condition number $\kappa$ for the preconditioned operator.

* (Block) Jacobi: $\delta=0$, $\kappa \sim H^{-2} H/h = (Hh)^{-1}$
* Overlapping Schwarz: $\kappa \sim H^{-2} H/\delta = (H \delta)^{-1}$
* 2-level overlapping Schwarz: $\kappa \sim H/\delta$

### Hands-on with PETSc: demonstrate these estimates

* Linear Poisson with geometric multigrid: `src/ksp/ksp/examples/tutorials/ex29.c`
* Nonlinear problems
  * Symmetric scalar problem: `src/snes/examples/tutorials/ex5.c`
  * Nonsymmetric system (lid/thermal-driven cavity): `src/snes/examples/tutorials/ex19.c`
* Compare preconditioned versus unpreconditioned norms.
* Compare BiCG versus GMRES
* Compare domain decomposition and multigrid preconditioning
 * `-pc_type asm` (Additive Schwarz)
  * `-pc_asm_type basic` (symmetric, versus `restrict`)
  * `-pc_asm_overlap 2` (increase overlap)
  * Effect of direct subdomain solver: `-sub_pc_type lu`
 * `-pc_type mg` (Geometric Multigrid)
* Use monitors:
 * `-ksp_monitor_true_residual`
 * `-ksp_monitor_singular_value`
 * `-ksp_converged_reason`
* Explain methods: `-snes_view`
* Performance info: `-log_view`

#### Example: Inhomogeneous Poisson

$$ -\nabla\cdot \Big( \rho(x,y) \nabla u(x,y) \Big) = e^{-10 (x^2 + y^2)} $$

in $\Omega = [0,1]^2$ with variable conductivity

$$
\rho(x,y) = \begin{cases}
\rho_0 & (x,y) \in [1/3, 2/3]^2 \\
1 & \text{otherwise}
\end{cases}
$$
    
where $\rho_0 > 0$ is a parameter (with default $\rho_0 = 1$).


```bash
%%bash

# You may need to change these for your machine
PETSC_DIR=$HOME/petsc PETSC_ARCH=ompi-optg

# Build the example
make -C $PETSC_DIR -f gmakefile $PETSC_ARCH/tests/ksp/ksp/examples/tutorials/ex29

# Link it from the current directory to make it easy to run below
cp -sf $PETSC_DIR/$PETSC_ARCH/tests/ksp/ksp/examples/tutorials/ex29 .
```

    make: Entering directory '/home/jed/petsc'
    make: 'ompi-optg/tests/ksp/ksp/examples/tutorials/ex29' is up to date.
    make: Leaving directory '/home/jed/petsc'



```python
# Prints solution DM and then a coordinate DM
! mpiexec -n 2 ./ex29 -da_refine 2 -dm_view
```

    DM Object: 2 MPI processes
      type: da
    Processor [0] M 9 N 9 m 1 n 2 w 1 s 1
    X range of indices: 0 9, Y range of indices: 0 5
    Processor [1] M 9 N 9 m 1 n 2 w 1 s 1
    X range of indices: 0 9, Y range of indices: 5 9
    DM Object: 2 MPI processes
      type: da
    Processor [0] M 9 N 9 m 1 n 2 w 2 s 1
    X range of indices: 0 9, Y range of indices: 0 5
    Processor [1] M 9 N 9 m 1 n 2 w 2 s 1
    X range of indices: 0 9, Y range of indices: 5 9



```python
! mpiexec -n 2 ./ex29 -rho 1e-1 -da_refine 3 -ksp_view_solution draw -draw_pause 5 -draw_cmap plasma
```

This problem is nonsymmetric due to boundary conditions, though symmetric solvers like CG and MINRES may still converge


```python
! mpiexec -n 2 ./ex29 -rho 1e-1 -da_refine 3 -ksp_monitor_true_residual -ksp_view -ksp_type gmres
```

      0 KSP preconditioned resid norm 1.338744788815e-02 true resid norm 1.433852280437e-02 ||r(i)||/||b|| 1.000000000000e+00
      1 KSP preconditioned resid norm 6.105013156491e-03 true resid norm 8.819020609674e-03 ||r(i)||/||b|| 6.150578222039e-01
      2 KSP preconditioned resid norm 3.380566739974e-03 true resid norm 3.966597605983e-03 ||r(i)||/||b|| 2.766392089410e-01
      3 KSP preconditioned resid norm 2.248884854426e-03 true resid norm 1.950654466953e-03 ||r(i)||/||b|| 1.360429169426e-01
      4 KSP preconditioned resid norm 1.603958727893e-03 true resid norm 1.729343487982e-03 ||r(i)||/||b|| 1.206082043163e-01
      5 KSP preconditioned resid norm 1.017005335066e-03 true resid norm 1.108652090238e-03 ||r(i)||/||b|| 7.731982613301e-02
      6 KSP preconditioned resid norm 5.817999897588e-04 true resid norm 7.954596575686e-04 ||r(i)||/||b|| 5.547709958842e-02
      7 KSP preconditioned resid norm 3.102671011646e-04 true resid norm 4.651546500795e-04 ||r(i)||/||b|| 3.244090457755e-02
      8 KSP preconditioned resid norm 1.547863442961e-04 true resid norm 2.154582266646e-04 ||r(i)||/||b|| 1.502652885547e-02
      9 KSP preconditioned resid norm 7.772941255716e-05 true resid norm 1.166482147907e-04 ||r(i)||/||b|| 8.135302107631e-03
     10 KSP preconditioned resid norm 3.800559054824e-05 true resid norm 5.777187067722e-05 ||r(i)||/||b|| 4.029136854992e-03
     11 KSP preconditioned resid norm 1.694315416916e-05 true resid norm 3.229096611633e-05 ||r(i)||/||b|| 2.252042735288e-03
     12 KSP preconditioned resid norm 6.705763692270e-06 true resid norm 1.252406213904e-05 ||r(i)||/||b|| 8.734555372208e-04
     13 KSP preconditioned resid norm 2.308568861148e-06 true resid norm 4.636253434420e-06 ||r(i)||/||b|| 3.233424738152e-04
     14 KSP preconditioned resid norm 8.946501825242e-07 true resid norm 1.703002880989e-06 ||r(i)||/||b|| 1.187711526650e-04
     15 KSP preconditioned resid norm 2.744515348301e-07 true resid norm 5.751960627589e-07 ||r(i)||/||b|| 4.011543382863e-05
     16 KSP preconditioned resid norm 1.137618031844e-07 true resid norm 2.081989399152e-07 ||r(i)||/||b|| 1.452025029048e-05
    KSP Object: 2 MPI processes
      type: gmres
        restart=30, using Classical (unmodified) Gram-Schmidt Orthogonalization with no iterative refinement
        happy breakdown tolerance 1e-30
      maximum iterations=10000, initial guess is zero
      tolerances:  relative=1e-05, absolute=1e-50, divergence=10000.
      left preconditioning
      using PRECONDITIONED norm type for convergence test
    PC Object: 2 MPI processes
      type: bjacobi
        number of blocks = 2
        Local solve is same for all blocks, in the following KSP and PC objects:
      KSP Object: (sub_) 1 MPI processes
        type: preonly
        maximum iterations=10000, initial guess is zero
        tolerances:  relative=1e-05, absolute=1e-50, divergence=10000.
        left preconditioning
        using NONE norm type for convergence test
      PC Object: (sub_) 1 MPI processes
        type: ilu
          out-of-place factorization
          0 levels of fill
          tolerance for zero pivot 2.22045e-14
          matrix ordering: natural
          factor fill ratio given 1., needed 1.
            Factored matrix follows:
              Mat Object: 1 MPI processes
                type: seqaij
                rows=153, cols=153
                package used to perform factorization: petsc
                total: nonzeros=713, allocated nonzeros=713
                total number of mallocs used during MatSetValues calls =0
                  not using I-node routines
        linear system matrix = precond matrix:
        Mat Object: 1 MPI processes
          type: seqaij
          rows=153, cols=153
          total: nonzeros=713, allocated nonzeros=713
          total number of mallocs used during MatSetValues calls =0
            not using I-node routines
      linear system matrix = precond matrix:
      Mat Object: 2 MPI processes
        type: mpiaij
        rows=289, cols=289
        total: nonzeros=1377, allocated nonzeros=1377
        total number of mallocs used during MatSetValues calls =0


### Default parallel solver

* Krylov method: GMRES
  * restart length of 30 to bound memory requirement and orthogonalization cost
  * classical Gram-Schmidt (compare `-ksp_gmres_modifiedgramschmidt`)
  * left preconditioning, uses preconditioned norm
  $$ P^{-1} A x = P^{-1} b $$
  * `-ksp_norm_type unpreconditioned`
  $$ A P^{-1} (P x) = b $$
  * Can estimate condition number using Hessenberg matrix
    * `-ksp_monitor_singular_value`
    * `-ksp_view_singularvalues`
    * Contaminated by restarts, so turn off restart `-ksp_gmres_restart 1000` for accurate results
* Preconditioner: block Jacobi
  * Expect condition number to scale with $1/(H h)$ where $H$ is the subdomain diameter and $h$ is the element size
  * One block per MPI process
    * No extra memory to create subdomain problems
    * Create two blocks per process: `-pc_bjacobi_local_blocks 2`
  * Each subdomain solver can be configured/monitored using the `-sub_` prefix
    * `-sub_ksp_type preonly` (default) means just apply the preconditioner
  * Incomplete LU factorization with zero fill
    * $O(n)$ cost to compute and apply; same memory as matrix $A$
    * gets weaker as $n$ increases
    * can fail unpredictably at the worst possible time
    * Allow "levels" of fill: `-sub_pc_factor_levels 2`
  * Try `-sub_pc_type lu`


```python
! mpiexec -n 2 ./ex29 -rho 1e-1 -da_refine 3 -ksp_monitor -ksp_view -sub_pc_factor_levels 3
```

      0 KSP Residual norm 3.321621226957e-02 
      1 KSP Residual norm 6.488371997792e-03 
      2 KSP Residual norm 3.872608843511e-03 
      3 KSP Residual norm 2.258796172567e-03 
      4 KSP Residual norm 6.146527388370e-04 
      5 KSP Residual norm 4.540373464970e-04 
      6 KSP Residual norm 1.994013489521e-04 
      7 KSP Residual norm 2.170446909144e-05 
      8 KSP Residual norm 7.079429242940e-06 
      9 KSP Residual norm 2.372198219605e-06 
     10 KSP Residual norm 9.203675161062e-07 
     11 KSP Residual norm 2.924907588760e-07 
    KSP Object: 2 MPI processes
      type: gmres
        restart=30, using Classical (unmodified) Gram-Schmidt Orthogonalization with no iterative refinement
        happy breakdown tolerance 1e-30
      maximum iterations=10000, initial guess is zero
      tolerances:  relative=1e-05, absolute=1e-50, divergence=10000.
      left preconditioning
      using PRECONDITIONED norm type for convergence test
    PC Object: 2 MPI processes
      type: bjacobi
        number of blocks = 2
        Local solve is same for all blocks, in the following KSP and PC objects:
      KSP Object: (sub_) 1 MPI processes
        type: preonly
        maximum iterations=10000, initial guess is zero
        tolerances:  relative=1e-05, absolute=1e-50, divergence=10000.
        left preconditioning
        using NONE norm type for convergence test
      PC Object: (sub_) 1 MPI processes
        type: ilu
          out-of-place factorization
          3 levels of fill
          tolerance for zero pivot 2.22045e-14
          matrix ordering: natural
          factor fill ratio given 1., needed 2.34642
            Factored matrix follows:
              Mat Object: 1 MPI processes
                type: seqaij
                rows=153, cols=153
                package used to perform factorization: petsc
                total: nonzeros=1673, allocated nonzeros=1673
                total number of mallocs used during MatSetValues calls =0
                  not using I-node routines
        linear system matrix = precond matrix:
        Mat Object: 1 MPI processes
          type: seqaij
          rows=153, cols=153
          total: nonzeros=713, allocated nonzeros=713
          total number of mallocs used during MatSetValues calls =0
            not using I-node routines
      linear system matrix = precond matrix:
      Mat Object: 2 MPI processes
        type: mpiaij
        rows=289, cols=289
        total: nonzeros=1377, allocated nonzeros=1377
        total number of mallocs used during MatSetValues calls =0


### Scaling estimates

#### Dependence on $h$


```python
! mpiexec -n 16 --oversubscribe ./ex29 -da_refine 3 -sub_pc_type lu -ksp_gmres_restart 1000 -ksp_converged_reason -ksp_view_singularvalues
```

    Linear solve converged due to CONVERGED_RTOL iterations 20
    Iteratively computed extreme singular values: max 1.9384 min 0.0694711 max/min 27.9023



```bash
%%bash

for refine in {4..8}; do
  mpiexec -n 16 --oversubscribe ./ex29 -da_refine $refine -sub_pc_type lu -ksp_gmres_restart 1000 -ksp_converged_reason -ksp_view_singularvalues
done
```

    Linear solve converged due to CONVERGED_RTOL iterations 27
    Iteratively computed extreme singular values: max 1.98356 min 0.0338842 max/min 58.5395
    Linear solve converged due to CONVERGED_RTOL iterations 36
    Iteratively computed extreme singular values: max 2.04703 min 0.0167502 max/min 122.209
    Linear solve converged due to CONVERGED_RTOL iterations 47
    Iteratively computed extreme singular values: max 2.12834 min 0.00830794 max/min 256.182
    Linear solve converged due to CONVERGED_RTOL iterations 62
    Iteratively computed extreme singular values: max 2.1865 min 0.00412757 max/min 529.731
    Linear solve converged due to CONVERGED_RTOL iterations 82
    Iteratively computed extreme singular values: max 2.22724 min 0.00206119 max/min 1080.56



```bash
%%bash

for refine in {3..8}; do
  mpiexec -n 16 --oversubscribe ./ex29 -da_refine $refine -pc_type asm -sub_pc_type lu -ksp_gmres_restart 1000 -ksp_converged_reason -ksp_view_singularvalues
done
```

    Linear solve converged due to CONVERGED_RTOL iterations 12
    Iteratively computed extreme singular values: max 1.39648 min 0.183011 max/min 7.63057
    Linear solve converged due to CONVERGED_RTOL iterations 16
    Iteratively computed extreme singular values: max 1.68852 min 0.0984075 max/min 17.1584
    Linear solve converged due to CONVERGED_RTOL iterations 23
    Iteratively computed extreme singular values: max 1.8569 min 0.0494302 max/min 37.5661
    Linear solve converged due to CONVERGED_RTOL iterations 31
    Iteratively computed extreme singular values: max 1.9503 min 0.0247646 max/min 78.7537
    Linear solve converged due to CONVERGED_RTOL iterations 41
    Iteratively computed extreme singular values: max 2.03979 min 0.0123563 max/min 165.081
    Linear solve converged due to CONVERGED_RTOL iterations 54
    Iteratively computed extreme singular values: max 2.12275 min 0.00615712 max/min 344.764



```bash
%%bash
cat > results.csv <<EOF
method,refine,its,cond
bjacobi,3,20,27.90
bjacobi,4,27,58.54
bjacobi,5,36,122.2
bjacobi,6,47,256.2
bjacobi,7,62,529.7
bjacobi,8,82,1080.6
asm,3,12,7.63
asm,4,16,17.15
asm,5,23,37.57
asm,6,31,78.75
asm,7,41,165.1
asm,8,54,344.8
EOF
```


```python
%matplotlib inline
import pandas
import seaborn
import matplotlib.pyplot as plt
plt.style.use('seaborn')

df = pandas.read_csv('results.csv')
n1 = 2**(df.refine + 1) # number of points per dimension
df['P'] = 16      # number of processes
df['N'] = n1**2    # number of dofs in global problem
df['h'] = 1/n1
df['H'] = 0.25 # 16 procs = 4x4 process grid
df['1/Hh'] = 1/(df.H * df.h)
```


```python
seaborn.lmplot(x='1/Hh', y='cond', hue='method', data=df)
df
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>method</th>
      <th>refine</th>
      <th>its</th>
      <th>cond</th>
      <th>P</th>
      <th>N</th>
      <th>h</th>
      <th>H</th>
      <th>1/Hh</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>0</td>
      <td>bjacobi</td>
      <td>3</td>
      <td>20</td>
      <td>27.90</td>
      <td>16</td>
      <td>256</td>
      <td>0.062500</td>
      <td>0.25</td>
      <td>64.0</td>
    </tr>
    <tr>
      <td>1</td>
      <td>bjacobi</td>
      <td>4</td>
      <td>27</td>
      <td>58.54</td>
      <td>16</td>
      <td>1024</td>
      <td>0.031250</td>
      <td>0.25</td>
      <td>128.0</td>
    </tr>
    <tr>
      <td>2</td>
      <td>bjacobi</td>
      <td>5</td>
      <td>36</td>
      <td>122.20</td>
      <td>16</td>
      <td>4096</td>
      <td>0.015625</td>
      <td>0.25</td>
      <td>256.0</td>
    </tr>
    <tr>
      <td>3</td>
      <td>bjacobi</td>
      <td>6</td>
      <td>47</td>
      <td>256.20</td>
      <td>16</td>
      <td>16384</td>
      <td>0.007812</td>
      <td>0.25</td>
      <td>512.0</td>
    </tr>
    <tr>
      <td>4</td>
      <td>bjacobi</td>
      <td>7</td>
      <td>62</td>
      <td>529.70</td>
      <td>16</td>
      <td>65536</td>
      <td>0.003906</td>
      <td>0.25</td>
      <td>1024.0</td>
    </tr>
    <tr>
      <td>5</td>
      <td>bjacobi</td>
      <td>8</td>
      <td>82</td>
      <td>1080.60</td>
      <td>16</td>
      <td>262144</td>
      <td>0.001953</td>
      <td>0.25</td>
      <td>2048.0</td>
    </tr>
    <tr>
      <td>6</td>
      <td>asm</td>
      <td>3</td>
      <td>12</td>
      <td>7.63</td>
      <td>16</td>
      <td>256</td>
      <td>0.062500</td>
      <td>0.25</td>
      <td>64.0</td>
    </tr>
    <tr>
      <td>7</td>
      <td>asm</td>
      <td>4</td>
      <td>16</td>
      <td>17.15</td>
      <td>16</td>
      <td>1024</td>
      <td>0.031250</td>
      <td>0.25</td>
      <td>128.0</td>
    </tr>
    <tr>
      <td>8</td>
      <td>asm</td>
      <td>5</td>
      <td>23</td>
      <td>37.57</td>
      <td>16</td>
      <td>4096</td>
      <td>0.015625</td>
      <td>0.25</td>
      <td>256.0</td>
    </tr>
    <tr>
      <td>9</td>
      <td>asm</td>
      <td>6</td>
      <td>31</td>
      <td>78.75</td>
      <td>16</td>
      <td>16384</td>
      <td>0.007812</td>
      <td>0.25</td>
      <td>512.0</td>
    </tr>
    <tr>
      <td>10</td>
      <td>asm</td>
      <td>7</td>
      <td>41</td>
      <td>165.10</td>
      <td>16</td>
      <td>65536</td>
      <td>0.003906</td>
      <td>0.25</td>
      <td>1024.0</td>
    </tr>
    <tr>
      <td>11</td>
      <td>asm</td>
      <td>8</td>
      <td>54</td>
      <td>344.80</td>
      <td>16</td>
      <td>262144</td>
      <td>0.001953</td>
      <td>0.25</td>
      <td>2048.0</td>
    </tr>
  </tbody>
</table>
</div>




![png](./lecture_15_1.png)



```python
import numpy as np
df['1/sqrt(Hh)'] = np.sqrt(df['1/Hh'])
plt.rc('figure', figsize=(16, 9))
g = seaborn.lmplot(x='1/sqrt(Hh)', y='its', hue='method', data=df);
```


![png](./lecture_16_0.png)



```python
from scipy.stats import linregress

bjacobi = df[df.method == 'bjacobi']
asm = df[df.method == 'asm']
bjacobi_its = linregress(bjacobi['1/sqrt(Hh)'], bjacobi['its'])
asm_its = linregress(asm['1/sqrt(Hh)'], asm['its'])
bjacobi_its, asm_its
```




    (LinregressResult(slope=1.649535193425483, intercept=8.498251134591065, rvalue=0.998633733566655, pvalue=2.7987507565052237e-06, stderr=0.043157836341681514),
     LinregressResult(slope=1.130138246216531, intercept=4.034979240523391, rvalue=0.9973956316037952, pvalue=1.0165269744723222e-05, stderr=0.04086178847687418))



#### Cost
Let $n = N/P$ be the subdomain size and suppose $k$ iterations are needed.

* Matrix assembly scales like $O(n)$ (perfect parallelism)
* 2D factorization in each subdomain scales as $O(n^{3/2})$
* Preconditioner application scales like $O(n \log n)$
* Matrix multiplication scales like $O(n)$
* GMRES scales like $O(k^2 n) + O(k^2 \log P)$
  * With restart length $r \ll k$, GMRES scales with $O(krn) + O(kr\log P)$


```python
! mpiexec -n 2 --oversubscribe ./ex29 -da_refine 8 -pc_type asm -sub_pc_type lu -ksp_converged_reason -log_view
```

    Linear solve converged due to CONVERGED_RTOL iterations 25
    ************************************************************************************************************************
    ***             WIDEN YOUR WINDOW TO 120 CHARACTERS.  Use 'enscript -r -fCourier9' to print this document            ***
    ************************************************************************************************************************
    
    ---------------------------------------------- PETSc Performance Summary: ----------------------------------------------
    
    ./ex29 on a ompi-optg named joule.int.colorado.edu with 2 processors, by jed Wed Oct 16 10:57:30 2019
    Using Petsc Development GIT revision: v3.12-32-g78b8d9f084  GIT Date: 2019-10-03 10:45:44 -0500
    
                             Max       Max/Min     Avg       Total 
    Time (sec):           1.484e+00     1.000   1.484e+00
    Objects:              1.040e+02     1.000   1.040e+02
    Flop:                 1.432e+09     1.004   1.429e+09  2.857e+09
    Flop/sec:             9.647e+08     1.004   9.628e+08  1.926e+09
    MPI Messages:         6.200e+01     1.000   6.200e+01  1.240e+02
    MPI Message Lengths:  2.524e+05     1.000   4.071e+03  5.048e+05
    MPI Reductions:       1.710e+02     1.000
    
    Flop counting convention: 1 flop = 1 real number operation of type (multiply/divide/add/subtract)
                                e.g., VecAXPY() for real vectors of length N --> 2N flop
                                and VecAXPY() for complex vectors of length N --> 8N flop
    
    Summary of Stages:   ----- Time ------  ----- Flop ------  --- Messages ---  -- Message Lengths --  -- Reductions --
                            Avg     %Total     Avg     %Total    Count   %Total     Avg         %Total    Count   %Total 
     0:      Main Stage: 1.4839e+00 100.0%  2.8574e+09 100.0%  1.240e+02 100.0%  4.071e+03      100.0%  1.630e+02  95.3% 
    
    ------------------------------------------------------------------------------------------------------------------------
    See the 'Profiling' chapter of the users' manual for details on interpreting output.
    Phase summary info:
       Count: number of times phase was executed
       Time and Flop: Max - maximum over all processors
                      Ratio - ratio of maximum to minimum over all processors
       Mess: number of messages sent
       AvgLen: average message length (bytes)
       Reduct: number of global reductions
       Global: entire computation
       Stage: stages of a computation. Set stages with PetscLogStagePush() and PetscLogStagePop().
          %T - percent time in this phase         %F - percent flop in this phase
          %M - percent messages in this phase     %L - percent message lengths in this phase
          %R - percent reductions in this phase
       Total Mflop/s: 10e-6 * (sum of flop over all processors)/(max time over all processors)
    ------------------------------------------------------------------------------------------------------------------------
    Event                Count      Time (sec)     Flop                              --- Global ---  --- Stage ----  Total
                       Max Ratio  Max     Ratio   Max  Ratio  Mess   AvgLen  Reduct  %T %F %M %L %R  %T %F %M %L %R Mflop/s
    ------------------------------------------------------------------------------------------------------------------------
    
    --- Event Stage 0: Main Stage
    
    BuildTwoSided          5 1.0 1.5282e-02 1.7 0.00e+00 0.0 4.0e+00 4.0e+00 0.0e+00  1  0  3  0  0   1  0  3  0  0     0
    BuildTwoSidedF         4 1.0 1.1949e-0217.8 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    MatMult               25 1.0 2.8539e-02 1.0 2.96e+07 1.0 5.0e+01 4.1e+03 0.0e+00  2  2 40 41  0   2  2 40 41  0  2071
    MatSolve              26 1.0 2.9259e-01 1.0 3.50e+08 1.0 0.0e+00 0.0e+00 0.0e+00 20 25  0  0  0  20 25  0  0  0  2393
    MatLUFactorSym         1 1.0 1.5648e-01 1.2 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00 10  0  0  0  0  10  0  0  0  0     0
    MatLUFactorNum         1 1.0 5.9458e-01 1.1 8.64e+08 1.0 0.0e+00 0.0e+00 0.0e+00 39 60  0  0  0  39 60  0  0  0  2896
    MatAssemblyBegin       3 1.0 1.0730e-0282.8 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    MatAssemblyEnd         3 1.0 9.8794e-03 1.1 0.00e+00 0.0 3.0e+00 1.4e+03 4.0e+00  1  0  2  1  2   1  0  2  1  2     0
    MatGetRowIJ            1 1.0 5.0642e-03 1.6 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    MatCreateSubMats       1 1.0 3.9036e-02 1.2 0.00e+00 0.0 1.0e+01 7.0e+03 1.0e+00  2  0  8 14  1   2  0  8 14  1     0
    MatGetOrdering         1 1.0 8.0494e-02 1.2 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  5  0  0  0  0   5  0  0  0  0     0
    MatIncreaseOvrlp       1 1.0 1.5691e-02 1.1 0.00e+00 0.0 0.0e+00 0.0e+00 1.0e+00  1  0  0  0  1   1  0  0  0  1     0
    KSPSetUp               2 1.0 2.8898e-03 1.0 0.00e+00 0.0 0.0e+00 0.0e+00 1.0e+01  0  0  0  0  6   0  0  0  0  6     0
    KSPSolve               1 1.0 1.2704e+00 1.0 1.43e+09 1.0 1.0e+02 4.1e+03 1.1e+02 86100 82 83 64  86100 82 83 67  2249
    KSPGMRESOrthog        25 1.0 8.4230e-02 1.0 1.71e+08 1.0 0.0e+00 0.0e+00 2.5e+01  6 12  0  0 15   6 12  0  0 15  4062
    DMCreateMat            1 1.0 6.0364e-02 1.0 0.00e+00 0.0 3.0e+00 1.4e+03 6.0e+00  4  0  2  1  4   4  0  2  1  4     0
    SFSetGraph             5 1.0 3.0582e-04 1.3 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    SFSetUp                5 1.0 3.2978e-02 1.5 0.00e+00 0.0 1.2e+01 1.4e+03 0.0e+00  2  0 10  3  0   2  0 10  3  0     0
    SFBcastOpBegin        51 1.0 7.6917e-03 1.0 0.00e+00 0.0 1.0e+02 4.1e+03 0.0e+00  1  0 82 83  0   1  0 82 83  0     0
    SFBcastOpEnd          51 1.0 1.0617e-02 1.9 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  1  0  0  0  0   1  0  0  0  0     0
    SFReduceBegin         26 1.0 5.9807e-03 1.3 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    SFReduceEnd           26 1.0 5.0625e-03 1.1 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    VecMDot               25 1.0 4.1009e-02 1.0 8.57e+07 1.0 0.0e+00 0.0e+00 2.5e+01  3  6  0  0 15   3  6  0  0 15  4171
    VecNorm               26 1.0 6.5928e-03 1.3 6.86e+06 1.0 0.0e+00 0.0e+00 2.6e+01  0  0  0  0 15   0  0  0  0 16  2076
    VecScale              26 1.0 2.2696e-03 1.0 3.43e+06 1.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0  3015
    VecCopy                1 1.0 1.2067e-04 1.1 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    VecSet                85 1.0 6.4445e-03 1.0 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    VecAXPY                1 1.0 1.7286e-04 1.0 2.64e+05 1.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0  3045
    VecMAXPY              26 1.0 4.5977e-02 1.0 9.23e+07 1.0 0.0e+00 0.0e+00 0.0e+00  3  6  0  0  0   3  6  0  0  0  4007
    VecAssemblyBegin       2 1.0 1.3040e-03 2.1 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    VecAssemblyEnd         2 1.0 4.9600e-06 1.4 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  0  0  0  0  0   0  0  0  0  0     0
    VecScatterBegin      129 1.0 2.7052e-02 1.0 0.00e+00 0.0 1.0e+02 4.1e+03 0.0e+00  2  0 82 83  0   2  0 82 83  0     0
    VecScatterEnd         77 1.0 1.5437e-02 1.4 0.00e+00 0.0 0.0e+00 0.0e+00 0.0e+00  1  0  0  0  0   1  0  0  0  0     0
    VecNormalize          26 1.0 8.8965e-03 1.2 1.03e+07 1.0 0.0e+00 0.0e+00 2.6e+01  1  1  0  0 15   1  1  0  0 16  2307
    PCSetUp                2 1.0 8.5827e-01 1.0 8.64e+08 1.0 1.3e+01 5.7e+03 7.0e+00 58 60 10 15  4  58 60 10 15  4  2006
    PCSetUpOnBlocks        1 1.0 7.9431e-01 1.0 8.64e+08 1.0 0.0e+00 0.0e+00 0.0e+00 53 60  0  0  0  53 60  0  0  0  2168
    PCApply               26 1.0 3.3956e-01 1.0 3.50e+08 1.0 5.2e+01 4.1e+03 0.0e+00 23 25 42 42  0  23 25 42 42  0  2062
    PCApplyOnBlocks       26 1.0 2.9531e-01 1.0 3.50e+08 1.0 0.0e+00 0.0e+00 0.0e+00 20 25  0  0  0  20 25  0  0  0  2371
    ------------------------------------------------------------------------------------------------------------------------
    
    Memory usage is given in bytes:
    
    Object Type          Creations   Destructions     Memory  Descendants' Mem.
    Reports information only for process 0.
    
    --- Event Stage 0: Main Stage
    
           Krylov Solver     2              2        20056     0.
         DMKSP interface     1              1          664     0.
                  Matrix     5              5    105275836     0.
        Distributed Mesh     3              3        15760     0.
               Index Set    17             17      5309508     0.
       IS L to G Mapping     3              3      2119704     0.
       Star Forest Graph    11             11        10648     0.
         Discrete System     3              3         2856     0.
                  Vector    50             50     45457728     0.
             Vec Scatter     5              5         4008     0.
          Preconditioner     2              2         2000     0.
                  Viewer     2              1          848     0.
    ========================================================================================================================
    Average time to get PetscTime(): 3.32e-08
    Average time for MPI_Barrier(): 1.404e-06
    Average time for zero size MPI_Send(): 8.8545e-06
    #PETSc Option Table entries:
    -da_refine 8
    -ksp_converged_reason
    -log_view
    -malloc_test
    -pc_type asm
    -sub_pc_type lu
    #End of PETSc Option Table entries
    Compiled without FORTRAN kernels
    Compiled with full precision matrices (default)
    sizeof(short) 2 sizeof(int) 4 sizeof(long) 8 sizeof(void*) 8 sizeof(PetscScalar) 8 sizeof(PetscInt) 4
    Configure options: --download-ctetgen --download-exodusii --download-hypre --download-ml --download-mumps --download-netcdf --download-pnetcdf --download-scalapack --download-sundials --download-superlu --download-superlu_dist --download-triangle --with-debugging=0 --with-hdf5 --with-med --with-metis --with-mpi-dir=/home/jed/usr/ccache/ompi --with-parmetis --with-suitesparse --with-x --with-zlib COPTFLAGS="-O2 -march=native -ftree-vectorize -g" PETSC_ARCH=ompi-optg
    -----------------------------------------
    Libraries compiled on 2019-10-03 21:38:02 on joule 
    Machine characteristics: Linux-5.3.1-arch1-1-ARCH-x86_64-with-arch
    Using PETSc directory: /home/jed/petsc
    Using PETSc arch: ompi-optg
    -----------------------------------------
    
    Using C compiler: /home/jed/usr/ccache/ompi/bin/mpicc  -fPIC -Wall -Wwrite-strings -Wno-strict-aliasing -Wno-unknown-pragmas -fstack-protector -fvisibility=hidden -O2 -march=native -ftree-vectorize -g  
    Using Fortran compiler: /home/jed/usr/ccache/ompi/bin/mpif90  -fPIC -Wall -ffree-line-length-0 -Wno-unused-dummy-argument -g -O    
    -----------------------------------------
    
    Using include paths: -I/home/jed/petsc/include -I/home/jed/petsc/ompi-optg/include -I/home/jed/usr/ccache/ompi/include
    -----------------------------------------
    
    Using C linker: /home/jed/usr/ccache/ompi/bin/mpicc
    Using Fortran linker: /home/jed/usr/ccache/ompi/bin/mpif90
    Using libraries: -Wl,-rpath,/home/jed/petsc/ompi-optg/lib -L/home/jed/petsc/ompi-optg/lib -lpetsc -Wl,-rpath,/home/jed/petsc/ompi-optg/lib -L/home/jed/petsc/ompi-optg/lib -Wl,-rpath,/usr/lib/openmpi -L/usr/lib/openmpi -Wl,-rpath,/usr/lib/gcc/x86_64-pc-linux-gnu/9.1.0 -L/usr/lib/gcc/x86_64-pc-linux-gnu/9.1.0 -lHYPRE -lcmumps -ldmumps -lsmumps -lzmumps -lmumps_common -lpord -lscalapack -lumfpack -lklu -lcholmod -lbtf -lccolamd -lcolamd -lcamd -lamd -lsuitesparseconfig -lsuperlu -lsuperlu_dist -lml -lsundials_cvode -lsundials_nvecserial -lsundials_nvecparallel -llapack -lblas -lexodus -lnetcdf -lpnetcdf -lmedC -lmed -lhdf5hl_fortran -lhdf5_fortran -lhdf5_hl -lhdf5 -lparmetis -lmetis -ltriangle -lm -lz -lX11 -lctetgen -lstdc++ -ldl -lmpi_usempif08 -lmpi_usempi_ignore_tkr -lmpi_mpifh -lmpi -lgfortran -lm -lgfortran -lm -lgcc_s -lquadmath -lpthread -lquadmath -lstdc++ -ldl
    -----------------------------------------
    


## Suggested exercises

* There is no substitute for experimentation.  Try some different methods or a different example.  How do the constants and scaling compare?
* Can you estimate parameters to model the leading costs for this solver?
  * In your model, how does degrees of freedom solved per second per process depend on discretization size $h$?
  * What would be optimal?
  
## An approach to modeling

We have conducted:
* a few experiments to validate the mathematical estimates for condition number and number of iterations
  * even when the theorem exists, we need to confirm that we're in the asymptotic regime
  * execution time does not matter at all for this; it's checking the math/convergence rates
  * indeed, we ran oversubscribed (more processes than cores)
* one performance experiment on representative hardware (`-log_view`)
  * this was on my laptop, but could have been on a couple nodes of a cluster

The performance experiment(s) enables us to fill in constants to estimate time.
These models could be more sophisticated, accounting for cache sizes, message sizes, and the like.
We implement this performance model in code, dropping in the timing constants from the experiment above, then run synthetic experiments to map out estimated performance in various scaling modes.


```python
def perf_model(h, P, method, regress_its):
    n1 = 1/h
    N = n1**2
    n = N / P
    H = 1/np.sqrt(P)
    its = regress_its.intercept + regress_its.slope / np.sqrt(H*h)
    nref = (2**9 + 1)**2/2 # 8 levels of refinement from a 3x3 (2x2 element) grid
    pc_setup = 8.5827e-01 / nref**1.5 * n**1.5
    pc_apply = 3.3956e-01 / 26 / (nref * np.log(nref)) * (n * np.log(n))
    matmult =  2.8539e-02 / 25 / nref * n
    gmres = 8.4230e-02 / 25 / nref * n + 30e-6
    total = pc_setup + its*(pc_apply + matmult + gmres)
    return dict(h=h, H=H, n=n, N=N, P=P, method=method, its=its,
                pc_setup=pc_setup, pc_apply=its*pc_apply, matmult=its*matmult,
                gmres=its*gmres)

mdf = pandas.DataFrame(columns='h H n N P method its pc_setup pc_apply matmult gmres'.split())
mdf.append(perf_model(.5**9, 2, 'asm', asm_its), ignore_index=True)
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>h</th>
      <th>H</th>
      <th>n</th>
      <th>N</th>
      <th>P</th>
      <th>method</th>
      <th>its</th>
      <th>pc_setup</th>
      <th>pc_apply</th>
      <th>matmult</th>
      <th>gmres</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>0</td>
      <td>0.001953</td>
      <td>0.707107</td>
      <td>131072.0</td>
      <td>262144.0</td>
      <td>2</td>
      <td>asm</td>
      <td>34.445514</td>
      <td>0.853261</td>
      <td>0.447958</td>
      <td>0.039168</td>
      <td>0.116635</td>
    </tr>
  </tbody>
</table>
</div>



### Weak scaling study: fixed subdomain size


```python
def make_stats(df):
    df['total'] = df.pc_setup + df.pc_apply + df.matmult + df.gmres
    df['cost'] = df.total * df.P
    df['efficiency'] = df.N / df.cost
    df['digits_accuracy'] = -np.log10(100*df.h**2)
    df['log10_time'] = np.log10(df.total)
    df['log10_cost'] = np.log10(df.cost)

mdf = pandas.DataFrame(columns='h H n N P method its pc_setup pc_apply matmult gmres'.split())
for h in [.002]:
    for scale in np.geomspace(2, 1e3, 10):
        for method, its in [('asm', asm_its), ('bjacobi', bjacobi_its)]:
            mdf = mdf.append(perf_model(h/scale, scale**2, method, its), ignore_index=True)
make_stats(mdf)

plt.rc('figure', figsize=(16,9))
grid = seaborn.scatterplot(x='P', y='total', style='method', hue='digits_accuracy', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(2, 2e6), yscale='log', ylim=(1, 2e3))
mdf
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>h</th>
      <th>H</th>
      <th>n</th>
      <th>N</th>
      <th>P</th>
      <th>method</th>
      <th>its</th>
      <th>pc_setup</th>
      <th>pc_apply</th>
      <th>matmult</th>
      <th>gmres</th>
      <th>total</th>
      <th>cost</th>
      <th>efficiency</th>
      <th>digits_accuracy</th>
      <th>log10_time</th>
      <th>log10_cost</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>0</td>
      <td>0.001000</td>
      <td>0.500000</td>
      <td>250000.0</td>
      <td>1.000000e+06</td>
      <td>4.000000</td>
      <td>asm</td>
      <td>54.576298</td>
      <td>2.24764</td>
      <td>1.427934</td>
      <td>0.118369</td>
      <td>0.350992</td>
      <td>4.144935</td>
      <td>1.657974e+01</td>
      <td>60314.577430</td>
      <td>4.000000</td>
      <td>0.617518</td>
      <td>1.219578</td>
    </tr>
    <tr>
      <td>1</td>
      <td>0.001000</td>
      <td>0.500000</td>
      <td>250000.0</td>
      <td>1.000000e+06</td>
      <td>4.000000</td>
      <td>bjacobi</td>
      <td>82.267708</td>
      <td>2.24764</td>
      <td>2.152452</td>
      <td>0.178428</td>
      <td>0.529081</td>
      <td>5.107601</td>
      <td>2.043040e+01</td>
      <td>48946.656533</td>
      <td>4.000000</td>
      <td>0.708217</td>
      <td>1.310277</td>
    </tr>
    <tr>
      <td>2</td>
      <td>0.000501</td>
      <td>0.250660</td>
      <td>250000.0</td>
      <td>3.978974e+06</td>
      <td>15.915896</td>
      <td>asm</td>
      <td>104.851598</td>
      <td>2.24764</td>
      <td>2.743336</td>
      <td>0.227410</td>
      <td>0.674323</td>
      <td>5.892709</td>
      <td>9.378775e+01</td>
      <td>42425.307545</td>
      <td>4.599771</td>
      <td>0.770315</td>
      <td>1.972146</td>
    </tr>
    <tr>
      <td>3</td>
      <td>0.000501</td>
      <td>0.250660</td>
      <td>250000.0</td>
      <td>3.978974e+06</td>
      <td>15.915896</td>
      <td>bjacobi</td>
      <td>155.648886</td>
      <td>2.24764</td>
      <td>4.072396</td>
      <td>0.337583</td>
      <td>1.001011</td>
      <td>7.658630</td>
      <td>1.218940e+02</td>
      <td>32642.914395</td>
      <td>4.599771</td>
      <td>0.884151</td>
      <td>2.085982</td>
    </tr>
    <tr>
      <td>4</td>
      <td>0.000251</td>
      <td>0.125661</td>
      <td>250000.0</td>
      <td>1.583223e+07</td>
      <td>63.328940</td>
      <td>asm</td>
      <td>205.137578</td>
      <td>2.24764</td>
      <td>5.367218</td>
      <td>0.444917</td>
      <td>1.319283</td>
      <td>9.379058</td>
      <td>5.939658e+02</td>
      <td>26655.127609</td>
      <td>5.199542</td>
      <td>0.972159</td>
      <td>2.773761</td>
    </tr>
    <tr>
      <td>5</td>
      <td>0.000251</td>
      <td>0.125661</td>
      <td>250000.0</td>
      <td>1.583223e+07</td>
      <td>63.328940</td>
      <td>bjacobi</td>
      <td>302.025008</td>
      <td>2.24764</td>
      <td>7.902180</td>
      <td>0.655054</td>
      <td>1.942386</td>
      <td>12.747260</td>
      <td>8.072705e+02</td>
      <td>19612.057407</td>
      <td>5.199542</td>
      <td>1.105417</td>
      <td>2.907019</td>
    </tr>
    <tr>
      <td>6</td>
      <td>0.000126</td>
      <td>0.062996</td>
      <td>250000.0</td>
      <td>6.299605e+07</td>
      <td>251.984210</td>
      <td>asm</td>
      <td>405.181693</td>
      <td>2.24764</td>
      <td>10.601171</td>
      <td>0.878787</td>
      <td>2.605809</td>
      <td>16.333407</td>
      <td>4.115761e+03</td>
      <td>15306.053492</td>
      <td>5.799313</td>
      <td>1.213077</td>
      <td>3.614450</td>
    </tr>
    <tr>
      <td>7</td>
      <td>0.000126</td>
      <td>0.062996</td>
      <td>250000.0</td>
      <td>6.299605e+07</td>
      <td>251.984210</td>
      <td>bjacobi</td>
      <td>594.006815</td>
      <td>2.24764</td>
      <td>15.541589</td>
      <td>1.288325</td>
      <td>3.820182</td>
      <td>22.897737</td>
      <td>5.769868e+03</td>
      <td>10918.109272</td>
      <td>5.799313</td>
      <td>1.359793</td>
      <td>3.761166</td>
    </tr>
    <tr>
      <td>8</td>
      <td>0.000063</td>
      <td>0.031581</td>
      <td>250000.0</td>
      <td>2.506597e+08</td>
      <td>1002.638645</td>
      <td>asm</td>
      <td>804.217010</td>
      <td>2.24764</td>
      <td>21.041527</td>
      <td>1.744244</td>
      <td>5.172088</td>
      <td>30.205500</td>
      <td>3.028520e+04</td>
      <td>8276.638274</td>
      <td>6.399084</td>
      <td>1.480086</td>
      <td>4.481230</td>
    </tr>
    <tr>
      <td>9</td>
      <td>0.000063</td>
      <td>0.031581</td>
      <td>250000.0</td>
      <td>2.506597e+08</td>
      <td>1002.638645</td>
      <td>bjacobi</td>
      <td>1176.433613</td>
      <td>2.24764</td>
      <td>30.780200</td>
      <td>2.551534</td>
      <td>7.565891</td>
      <td>43.145266</td>
      <td>4.325911e+04</td>
      <td>5794.378472</td>
      <td>6.399084</td>
      <td>1.634933</td>
      <td>4.636078</td>
    </tr>
    <tr>
      <td>10</td>
      <td>0.000032</td>
      <td>0.015832</td>
      <td>250000.0</td>
      <td>9.973683e+08</td>
      <td>3989.473198</td>
      <td>asm</td>
      <td>1600.187362</td>
      <td>2.24764</td>
      <td>41.867289</td>
      <td>3.470602</td>
      <td>10.291141</td>
      <td>57.876673</td>
      <td>2.308974e+05</td>
      <td>4319.529584</td>
      <td>6.998856</td>
      <td>1.762504</td>
      <td>5.363419</td>
    </tr>
    <tr>
      <td>11</td>
      <td>0.000032</td>
      <td>0.015832</td>
      <td>250000.0</td>
      <td>9.973683e+08</td>
      <td>3989.473198</td>
      <td>bjacobi</td>
      <td>2338.221662</td>
      <td>2.24764</td>
      <td>61.177213</td>
      <td>5.071305</td>
      <td>15.037594</td>
      <td>83.533752</td>
      <td>3.332557e+05</td>
      <td>2992.802242</td>
      <td>6.998856</td>
      <td>1.921862</td>
      <td>5.522778</td>
    </tr>
    <tr>
      <td>12</td>
      <td>0.000016</td>
      <td>0.007937</td>
      <td>250000.0</td>
      <td>3.968503e+09</td>
      <td>15874.010520</td>
      <td>asm</td>
      <td>3187.938555</td>
      <td>2.24764</td>
      <td>83.409198</td>
      <td>6.914232</td>
      <td>20.502302</td>
      <td>113.073373</td>
      <td>1.794928e+06</td>
      <td>2210.953766</td>
      <td>7.598627</td>
      <td>2.053360</td>
      <td>6.254047</td>
    </tr>
    <tr>
      <td>13</td>
      <td>0.000016</td>
      <td>0.007937</td>
      <td>250000.0</td>
      <td>3.968503e+09</td>
      <td>15874.010520</td>
      <td>bjacobi</td>
      <td>4655.682804</td>
      <td>2.24764</td>
      <td>121.811247</td>
      <td>10.097582</td>
      <td>29.941673</td>
      <td>164.098143</td>
      <td>2.604896e+06</td>
      <td>1523.478544</td>
      <td>7.598627</td>
      <td>2.215104</td>
      <td>6.415790</td>
    </tr>
    <tr>
      <td>14</td>
      <td>0.000008</td>
      <td>0.003979</td>
      <td>250000.0</td>
      <td>1.579057e+10</td>
      <td>63162.276697</td>
      <td>asm</td>
      <td>6355.083968</td>
      <td>2.24764</td>
      <td>166.274365</td>
      <td>13.783367</td>
      <td>40.870879</td>
      <td>223.176251</td>
      <td>1.409632e+07</td>
      <td>1120.190874</td>
      <td>8.198398</td>
      <td>2.348648</td>
      <td>7.149106</td>
    </tr>
    <tr>
      <td>15</td>
      <td>0.000008</td>
      <td>0.003979</td>
      <td>250000.0</td>
      <td>1.579057e+10</td>
      <td>63162.276697</td>
      <td>bjacobi</td>
      <td>9278.407360</td>
      <td>2.24764</td>
      <td>242.760175</td>
      <td>20.123682</td>
      <td>59.671385</td>
      <td>324.802883</td>
      <td>2.051529e+07</td>
      <td>769.697602</td>
      <td>8.198398</td>
      <td>2.511620</td>
      <td>7.312078</td>
    </tr>
    <tr>
      <td>16</td>
      <td>0.000004</td>
      <td>0.001995</td>
      <td>250000.0</td>
      <td>6.283027e+10</td>
      <td>251321.062979</td>
      <td>asm</td>
      <td>12672.704838</td>
      <td>2.24764</td>
      <td>331.568547</td>
      <td>27.485481</td>
      <td>81.500824</td>
      <td>442.802492</td>
      <td>1.112856e+08</td>
      <td>564.585802</td>
      <td>8.798169</td>
      <td>2.646210</td>
      <td>8.046439</td>
    </tr>
    <tr>
      <td>17</td>
      <td>0.000004</td>
      <td>0.001995</td>
      <td>250000.0</td>
      <td>6.283027e+10</td>
      <td>251321.062979</td>
      <td>bjacobi</td>
      <td>18499.525217</td>
      <td>2.24764</td>
      <td>484.021428</td>
      <td>40.123111</td>
      <td>118.974329</td>
      <td>645.366508</td>
      <td>1.621942e+08</td>
      <td>387.376780</td>
      <td>8.798169</td>
      <td>2.809806</td>
      <td>8.210035</td>
    </tr>
    <tr>
      <td>18</td>
      <td>0.000002</td>
      <td>0.001000</td>
      <td>250000.0</td>
      <td>2.500000e+11</td>
      <td>1000000.000000</td>
      <td>asm</td>
      <td>25274.694404</td>
      <td>2.24764</td>
      <td>661.286899</td>
      <td>54.817589</td>
      <td>162.546864</td>
      <td>880.898993</td>
      <td>8.808990e+08</td>
      <td>283.800983</td>
      <td>9.397940</td>
      <td>2.944926</td>
      <td>8.944926</td>
    </tr>
    <tr>
      <td>19</td>
      <td>0.000002</td>
      <td>0.001000</td>
      <td>250000.0</td>
      <td>2.500000e+11</td>
      <td>1000000.000000</td>
      <td>bjacobi</td>
      <td>36893.226489</td>
      <td>2.24764</td>
      <td>965.274079</td>
      <td>80.016703</td>
      <td>237.268082</td>
      <td>1284.806505</td>
      <td>1.284807e+09</td>
      <td>194.581829</td>
      <td>9.397940</td>
      <td>3.108838</td>
      <td>9.108838</td>
    </tr>
  </tbody>
</table>
</div>




![png](./lecture_23_1.png)



```python
grid = seaborn.scatterplot(x='P', y='efficiency', style='method', hue='digits_accuracy', size='its', sizes=(50,400), data=mdf)
grid.axes.set(xscale='log', xlim=(3, 2e6));
```


![png](./lecture_24_0.png)


### Strong scaling study: fixed global problem size


```python
mdf = pandas.DataFrame(columns='h H n N P method its pc_setup pc_apply matmult gmres'.split())
for h in [1e-5]:
    for scale in np.geomspace(2, 1e3, 10):
        for method, its in [('asm', asm_its), ('bjacobi', bjacobi_its)]:
            mdf = mdf.append(perf_model(h, scale**2, method, its), ignore_index=True)
make_stats(mdf)

grid = seaborn.scatterplot(x='P', y='total', style='method', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(2, 2e6), yscale='log');
```


![png](./lecture_26_0.png)



```python
grid = seaborn.scatterplot(x='P', y='efficiency', style='method', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(2, 2e6), ylim=0);
```


![png](./lecture_27_0.png)


## Accessible range

Now we grid sample the space of possible configurations to understand the shape of the [Pareto front](https://en.wikipedia.org/wiki/Pareto_efficiency) -- those configurations that are optimal in terms of the multiple objectives, which we take to be
* number of digits of accuracy
  * scales with $O(h^2)$ for this discretization
  * sometimes we could change the discretization to converge as $O(h^3)$ or a higher power; these are called "higher order methods"
* total execution time: how long we have wait for the parallel computation to complete
* cost: how many node hours are we charged for
* efficiency: given the required problem size to reach desired accuracy, how well are the node resources being used
  * we measure here as number of degrees of freedom solved per second per core
  
All of these metrics have tangible units and none are input parameters.  We'll map out the best configurations possible within a class of algorithms and, after identifying a Pareto optimal case that we would like to run, can check the input parameters to see how to run it on a real machine.


```python
mdf = pandas.DataFrame(columns='h H n N P method its pc_setup pc_apply matmult gmres'.split())
for h in np.geomspace(1e-3, .1, 10):
    for scale in np.geomspace(2, 1e3, 10):
        for method, its in [('asm', asm_its), ('bjacobi', bjacobi_its)]:
            mdf = mdf.append(perf_model(h/scale, scale**2, method, its), ignore_index=True)
make_stats(mdf)

grid = seaborn.scatterplot(x='P', y='total', style='method', hue='digits_accuracy', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(3, 2e6), yscale='log', ylim=(1e-3, 2e5));
```


![png](./lecture_29_0.png)



```python
grid = seaborn.scatterplot(x='total', y='efficiency', style='method', hue='digits_accuracy', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(1e-3, 1e4), yscale='log');
```


![png](./lecture_30_0.png)



```python
grid = seaborn.scatterplot(x='digits_accuracy', y='efficiency', style='method', hue='log10_time', size='log10_cost', sizes=(30,400), data=mdf)
grid.axes.set(yscale='log');
```


![png](./lecture_31_0.png)



```python
grid = seaborn.scatterplot(x='total', y='digits_accuracy', style='method', size='log10_cost', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(1, 1e4), ylim=3);
```


![png](./lecture_32_0.png)


## Two-level methods

Two-level methods add a coarse space $P_0$ to the preconditioner

$$ M^{-1} = P_0 A_0^{-1} P_0^T + \sum_{i=1}^{\text{subdomains}} P_i A_i^{-1} P_i^T $$

or a similar combination.  In most formulations, the size of the coarse problem $A_0 = P_0^T A P_0$ is proportional to the number of subdomains.

Compare the figures below with those above to see how two-level methods open up new accessible ranges and bring their own tradeoffs.


```python
def perf_model_2level(h, P, method, regress_its):
    n1 = 1/h
    N = n1**2
    n = N / P
    H = 1/np.sqrt(P)
    its = regress_its.intercept + regress_its.slope / np.sqrt(H*h) * H
    nref = (2**9 + 1)**2/2 # 8 levels of refinement from a 3x3 (2x2 element) grid
    pc_setup = 8.5827e-01 / nref**1.5 * n**1.5
    pc_setup += 8.5827e-01 / nref**1.5 * (4*P)**1.5
    pc_apply = 3.3956e-01 / 26 / (nref * np.log(nref)) * (n * np.log(n) + 4*P * np.log(4*P))
    matmult =  2.8539e-02 / 25 / nref * n
    gmres = 8.4230e-02 / 25 / nref * n + 30e-6
    total = pc_setup + its*(pc_apply + matmult + gmres)
    return dict(h=h, H=H, n=n, N=N, P=P, method=method, its=its,
                pc_setup=pc_setup, pc_apply=its*pc_apply, matmult=its*matmult,
                gmres=its*gmres)
```

### Strong scaling


```python
mdf = pandas.DataFrame(columns='h H n N P method its pc_setup pc_apply matmult gmres'.split())
for h in [1e-5]:
    for scale in np.geomspace(2, 1e3, 10):
        for method, its in [('asm', asm_its), ('bjacobi', bjacobi_its)]:
            mdf = mdf.append(perf_model_2lev(h, scale**2, method, its), ignore_index=True)
make_stats(mdf)
grid = seaborn.scatterplot(x='P', y='efficiency', style='method', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(2, 2e6), ylim=0);
```


![png](./lecture_36_0.png)


* Why does efficiency drop off on the left?
* Why does it drop off on the right?

### Accessible range


```python
mdf = pandas.DataFrame(columns='h H n N P method its pc_setup pc_apply matmult gmres'.split())
for h in np.geomspace(1e-3, .1, 10):
    for scale in np.geomspace(2, 1e3, 10):
        for method, its in [('asm', asm_its), ('bjacobi', bjacobi_its)]:
            mdf = mdf.append(perf_model_2level(h/scale, scale**2, method, its), ignore_index=True)
make_stats(mdf)

grid = seaborn.scatterplot(x='P', y='total', style='method', hue='digits_accuracy', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(3, 2e6), yscale='log', ylim=(1e-3, 2e5));
```


![png](./lecture_38_0.png)



```python
grid = seaborn.scatterplot(x='total', y='efficiency', style='method', hue='digits_accuracy', size='its', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(1e-3, 1e4), yscale='log');
```


![png](./lecture_39_0.png)



```python
grid = seaborn.scatterplot(x='digits_accuracy', y='efficiency', style='method', hue='log10_time', size='log10_cost', sizes=(30,400), data=mdf)
grid.axes.set(yscale='log');
```


![png](./lecture_40_0.png)



```python
grid = seaborn.scatterplot(x='total', y='digits_accuracy', style='method', size='log10_cost', sizes=(30,400), data=mdf)
grid.axes.set(xscale='log', xlim=(1, 1e4), ylim=3);
```


![png](./lecture_41_0.png)

