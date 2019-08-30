---
title: "Intro to Vectorization"
toc: true
title: Vectorization and Instruction-Level Parallelism
date: 2019-08-30T11:00:00-06:00
type: docs
weight: 2
menu:
  fall2019:
    parent: Lecture Notes
    name: 2019-08-30 Vectorization
---

Remember how single-thread performance has increased significantly
since ~2004 when clock frequency stagnated?

![42 years of microprocessor data](https://www.karlrupp.net/wp-content/uploads/2018/02/42-years-processor-trend.png)

This is a result of doing more per clock cycle.

![Flops per clock cycle](https://www.karlrupp.net/wp-content/uploads/2013/06/flops-per-cycle-sp.png)

Let's visit some slides:

* [Georg Hager (2019): Modern Computer Architucture](https://moodle.rrze.uni-erlangen.de/pluginfile.php/12916/mod_resource/content/6/01_IntroArchitecture.pdf)


## Further resources

* [Intel Intrinsics Guide](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#)
* Wikichip
  * [Intel Xeon: Cascade Lake](https://en.wikichip.org/wiki/intel/microarchitectures/cascade_lake)
  * [AMD EPYC gen2: Rome](https://en.wikichip.org/wiki/amd/cores/rome)
  * [IBM POWER9](https://en.wikichip.org/wiki/ibm/microarchitectures/power9)
* [Agner Fog's website](https://www.agner.org/optimize/)
