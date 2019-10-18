---
title: "Nonlinear and transient problems"
date: 2019-10-21T06:49:25-06:00
toc: true
markup: mmark
type: docs
weight: 2
menu:
  fall2019:
    parent: Lecture Notes
    name: 2019-10-21 Nonlinear/transient
---

```python
%matplotlib inline
import pandas
import seaborn
import matplotlib.pyplot as plt
import numpy as np
plt.style.use('seaborn')
```

## Newton-Raphson methods for systems

Up to now, we have been solving linear problems.

The **Jacobian** of $F$ is
$$ J(u) = \frac{\partial F}{\partial u}(u) =
\begin{bmatrix} \frac{\partial F_0}{\partial u_0} & \frac{\partial F_0}{\partial u_1} & \dotsb \\
 \frac{\partial F_1}{\partial u_0} & \frac{\partial F_1}{\partial u_1} &  \\
 \vdots & & \ddots
 \end{bmatrix}(u) . $$
The method can be derived by taking the Taylor expansion of $F(u)$ at $u$,
$$ F(u + \delta u) = F(u) + \frac{\partial F}{\partial u}(u) (\delta u) + \frac{\partial^2 F}{\partial u^2}(u) (\delta u \otimes \delta u) / 2 + \dotsb $$
Note that each higher term is a higher rank tensor, thus computationally unweildy.  If we truncate the series with the linear term and set equal to zero, we have a linear equation for $\delta u$
$$ \frac{\partial F}{\partial u}(u) \delta u = - F(u) $$
which will hopefully make $F(u + \partial u) \approx 0$.  This is Newton's method.

* Each iteration requires evaluating $F(u)$ -- almost any method will have this property.
* Each iteration requires evaluating the Jacobian matrix $J(u)$ -- this either requires custom code, algorithmic differentiation, or a finite difference approximation (we'll revisit this later).
* Each iteration requires solving a linear system with the matrix $J(u)$.  This may be expensive.


```python
def fsolve_newton(F, J, u0, rtol=1e-10, maxit=50, verbose=False):
    u = u0.copy()
    Fu = F(u)
    norm0 = np.linalg.norm(Fu)
    enorm_last = np.linalg.norm(u - np.array([1,1]))
    for i in range(maxit):
        du = -np.linalg.solve(J(u), Fu)
        u += du
        Fu = F(u)
        norm = np.linalg.norm(Fu)
        if verbose:
            enorm = np.linalg.norm(u - np.array([1,1]))
            print('Newton {:d} anorm {:6.2e} rnorm {:6.2e} eratio {:6.2f}'.
                  format(i+1, norm, norm/norm0, enorm/enorm_last**2))
            enorm_last = enorm
        if norm < rtol * norm0:
            break
    return u, i

def rostest(a,b):
    def F(u):
        x = u[0]; y = u[1]
        return np.array([-2*(a-x) + 4*b*x**3 - 4*b*x*y,
                         2*b*(y-x**2)])
    def J(u):
        x = u[0]; y = u[1]
        return np.array([[2 + 12*b*x**2 - 4*b*y, -4*b*x],
                         [-4*b*x, 2*b]])
    return F, J

F, J = rostest(1,3)
fsolve_newton(F, J, np.array([0, 1.]), verbose=True)
```

    Newton 1 anorm 2.51e+00 rnorm 3.96e-01 eratio   1.56
    Newton 2 anorm 9.91e+00 rnorm 1.57e+00 eratio   0.56
    Newton 3 anorm 3.83e-01 rnorm 6.05e-02 eratio   0.22
    Newton 4 anorm 5.11e-01 rnorm 8.08e-02 eratio   0.25
    Newton 5 anorm 5.24e-04 rnorm 8.28e-05 eratio   0.36
    Newton 6 anorm 9.76e-07 rnorm 1.54e-07 eratio   0.21
    Newton 7 anorm 3.61e-15 rnorm 5.72e-16 eratio   0.31





    (array([1., 1.]), 6)



* Can the iteration break down?  How?
* How does the method depend on the initial guess?
* It turns out that Newton's method has _locally quadratic_ convergence to simple roots, $$\lim_{i \to \infty} |e_{i+1}|/|e_i^2| < \infty .$$
* "The number of correct digits doubles each iteration."
* Now that we know how to make a good guess accurate, the effort lies in getting a good guess.

## Matrix-free Jacobian via finite differencing

It can be error-prone and complicated to implement the Jacobian function `J(u)`.  In such cases, we can use the approximation

$$ J(u) v \approx \frac{F(u+\epsilon v) - F(u)}{\epsilon} $$

where $\epsilon$ is some "small" number.  Now can't access individual entries of $J$, but we can apply its action to an arbitrary vector $u$.

We know that this approximation is first order accurate in $\epsilon$, 
$$ \left\lVert J(u) v - \frac{F(u+\epsilon v) - F(u)}{\epsilon} \right\rVert \in O(\epsilon) . $$
But if $\epsilon$ is too small, we will lose accuracy due to rounding error.  If $F$ has been scaled such that its norm is of order 1, then $\epsilon = \sqrt{\epsilon_{\text{machine}}}$ is a good default choice.


```python
import scipy.sparse.linalg as splinalg

def fsolve_newtonkrylov(F, u0, epsilon=1e-8, rtol=1e-10, maxit=50, verbose=False):
    u = u0.copy()
    Fu = F(u)
    norm0 = np.linalg.norm(Fu)
    for i in range(maxit):
        def Ju_fd(v):
            return (F(u + epsilon*v) - Fu) / epsilon
        Ju = splinalg.LinearOperator((len(Fu),len(u)), matvec=Ju_fd)
        du, info = splinalg.gmres(Ju, Fu, atol=1.e-6)
        if info != 0:
            print(np.linalg.norm(Ju @ du - Fu), norm)
            raise RuntimeError('GMRES failed to converge: {:d}'.format(info))
        u -= du
        Fu = F(u)
        norm = np.linalg.norm(Fu)
        if verbose:
            print('Newton {:d} anorm {:6.2e} rnorm {:6.2e}'
                  .format(i, norm, norm/norm0))
        if norm < rtol * norm0:
            break
    return u, i

fsolve_newtonkrylov(F, np.array([0.,1]), rtol=1e-6, verbose=True)
```

    Newton 0 anorm 2.51e+00 rnorm 3.96e-01
    Newton 1 anorm 9.91e+00 rnorm 1.57e+00
    Newton 2 anorm 3.83e-01 rnorm 6.05e-02
    Newton 3 anorm 5.11e-01 rnorm 8.08e-02
    Newton 4 anorm 5.24e-04 rnorm 8.28e-05
    Newton 5 anorm 9.76e-07 rnorm 1.54e-07





    (array([1.        , 0.99999992]), 5)



![](tme-ice-nk.png)

![](tme-ice-its.png)

![](tme-ice-breakdown.png)

## Further reading

* Brown, Smith, and Ahmadia (2013) [**Textbook multigrid efficiency for hydrostatic ice flow**](https://doi.org/10.1137/110834512)
  * Configuring an efficient nonlinear solver (see `snes/examples/tutorials/ex48.c` in PETSc repository)
