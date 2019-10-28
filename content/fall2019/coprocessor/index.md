---
title: "Coprocessor architectures"
date: 2019-10-28T06:49:25-06:00
toc: true
markup: mmark
type: docs
weight: 2
menu:
  fall2019:
    parent: Lecture Notes
    name: 2019-10-28 Coprocessors
---

## Coprocessor architectures

![](https://en.wikichip.org/w/images/0/06/summit_single-node.svg)

* CUDA devices (NVIDIA)
  * Programmable via **CUDA**, OpenACC, OpenMP-5, OpenCL, HIP->CUDA, SYCL->CUDA
  * Example machine: [OLCF Summit](https://en.wikichip.org/wiki/supercomputers/summit) (details from [user guide](https://www.olcf.ornl.gov/for-users/system-user-guides/summit/summit-user-guide/))
* ROCm devices (AMD)
  * Programmable via **HIP**, OpenMP-5, OpenCL, SYCL->HIP
  * Example machine: [OLCF Frontier](https://www.olcf.ornl.gov/wp-content/uploads/2019/05/frontier_specsheet_v4.pdf)
* Intel X GPUs
  * Programmable via **SYCL**, OpenMP-5, OpenCL?
  * Example machine: [ALCF Aurora/A21](https://aurora.alcf.anl.gov/)
* Upcoming non-coprocessor Supercomputers
  * [RIKEN Fugaku (Post-K)](https://www.hpcwire.com/2019/05/23/riken-post-k-supercomputer-named-after-japans-tallest-peak/)
  * [TACC Frontera](https://www.tacc.utexas.edu/systems/frontera)

## Fundamental capabilities


```python
from io import StringIO
import pandas
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')

data = StringIO("""
package,cores,lanes/core,clock (MHz),peak (GF),bandwidth (GB/s),TDP (W),MSRP
Xeon 8280,28,8,2700,2400,141,205,10000
NVIDIA V100,80,64,1455,7800,900,300,10664
AMD MI60,64,64,1800,7362,1024,300,
AMD Rome,64,4,2000,2048,205,200,6450
""")

df = pandas.read_csv(data, index_col='package')
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
      <th>cores</th>
      <th>lanes/core</th>
      <th>clock (MHz)</th>
      <th>peak (GF)</th>
      <th>bandwidth (GB/s)</th>
      <th>TDP (W)</th>
      <th>MSRP</th>
    </tr>
    <tr>
      <th>package</th>
      <th></th>
      <th></th>
      <th></th>
      <th></th>
      <th></th>
      <th></th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>Xeon 8280</th>
      <td>28</td>
      <td>8</td>
      <td>2700</td>
      <td>2400</td>
      <td>141</td>
      <td>205</td>
      <td>10000.0</td>
    </tr>
    <tr>
      <th>NVIDIA V100</th>
      <td>80</td>
      <td>64</td>
      <td>1455</td>
      <td>7800</td>
      <td>900</td>
      <td>300</td>
      <td>10664.0</td>
    </tr>
    <tr>
      <th>AMD MI60</th>
      <td>64</td>
      <td>64</td>
      <td>1800</td>
      <td>7362</td>
      <td>1024</td>
      <td>300</td>
      <td>NaN</td>
    </tr>
    <tr>
      <th>AMD Rome</th>
      <td>64</td>
      <td>4</td>
      <td>2000</td>
      <td>2048</td>
      <td>205</td>
      <td>200</td>
      <td>6450.0</td>
    </tr>
  </tbody>
</table>
</div>



### [Amdahl's Law](https://en.wikipedia.org/wiki/Amdahl%27s_law) for energy efficiency


```python
df['efficiency (GF/W)'] = df['peak (GF)'] / df['TDP (W)']
df['efficiency (GF/W)']
```




    package
    Xeon 8280      11.707317
    NVIDIA V100    26.000000
    AMD MI60       24.540000
    AMD Rome       10.240000
    Name: efficiency (GF/W), dtype: float64




```python
ngpu = np.arange(0, 9)
overhead = 100 # Power supply, DRAM, disk, etc.
peak = (ngpu == 0)*df.loc['Xeon 8280']['peak (GF)'] + ngpu*df.loc['NVIDIA V100']['peak (GF)']
tdp = overhead + df.loc['Xeon 8280']['TDP (W)'] + ngpu*df.loc['NVIDIA V100']['TDP (W)']
plt.plot(ngpu, peak / tdp)
plt.xlabel('number of GPUs per CPU')
plt.title('DP Peak efficiency (GF/W)');
```


![png](./lecture_5_0.png)


#### Compare to [Green 500 list](https://www.top500.org/green500/lists/2019/06/)
* [#1 system](https://www.top500.org/system/179166) is **15.1 GF/W** (2x Xeon E5-2698v4, 8x V100)
* [#2 system](https://www.top500.org/system/179397) (Summit) is **14.7 GF/W** (2x Power9, 6x V100)
* [#27 system](https://www.top500.org/system/179683) is **5.8 GF/W** on Xeon 6248 (no GPU)

### Amdahl for cost efficiency


```python
df['cost (GF/$)'] = df['peak (GF)'] / df['MSRP']
df['cost (GF/$)']
```




    package
    Xeon 8280      0.240000
    NVIDIA V100    0.731433
    AMD MI60            NaN
    AMD Rome       0.317519
    Name: cost (GF/$), dtype: float64




```python
overhead = 3000 + 2000*ngpu # power supply, memory, cooling, maintenance
cost = overhead + df.loc['Xeon 8280']['MSRP'] + ngpu*df.loc['NVIDIA V100']['MSRP']
plt.plot(ngpu, peak / cost)
plt.xlabel('number of GPUs per CPU')
plt.title('DP cost efficiency (GF/$)');
```


![png](./lecture_8_0.png)


### What fraction of datacenter cost goes to the power bill?

* OLCF Summit is reportedly a \$200M machine.
* What if we just buy the GPUs at retail?
  * 256 racks
  * 18 nodes per rack
  * 6 GPUs per node
  * V100 MSRP of about $10k


```python
256 * 18 * 6 * 10e3 / 1e6 # millions
```




    276.48



* Rule of thumb: $ \lesssim \$1M $ per MW-year
* We know Summit is a 13 MW facility
* Check [industrial electricity rates](https://www.electricitylocal.com/states/tennessee/knoxville/)
![](knoxville-electricity.png)


```python
.0638 * 24 * 365
```




    558.8879999999999



![](olcf-4-foia.png)

## Programming models

* Directives
  * [OpenMP-5](https://www.openmp.org/resources/refguides/)
  * [OpenACC](https://www.openacc.org/)

```c
#pragma acc data copy(A) create(Anew)
while ( error > tol  &&  iter  <  iter_max )  {
  error = 0.0;
#pragma acc kernels {
#pragma acc loop independent collapse(2)
  for (  int  j = 1; j < n-1;  j++ )  {
    for (  int  i = 1; i < m-1; i++ )  {
       Anew [j] [i] = 0.25 * ( A [j] [i+1] + A [j] [i-1] +
                                      A [j-1] [i] + A [j+1] [i]);
       error = max ( error, fabs (Anew [j] [i] - A [j] [i]));
      }
    }
  } 
}
```
Comparison slides: [Is OpenMP 4.5 Target Off-load Ready for Real Life? A Case Study of Three Benchmark Kernels (2018)](https://openmpcon.org/wp-content/uploads/2018_Session1_Diaz.pdf)
  
* Thread "kernel" and control
  * [CUDA](https://devblogs.nvidia.com/even-easier-introduction-cuda/)
  * [HIP](https://rocm-documentation.readthedocs.io/en/latest/Programming_Guides/HIP-GUIDE.html) ([video](https://vimeo.com/channels/olcftraining/359154970))
* C++ templated
  * [SYCL](https://www.khronos.org/sycl/)
  * [Kokkos](https://github.com/kokkos/kokkos)
  * [Raja](https://raja.readthedocs.io/en/master/)
