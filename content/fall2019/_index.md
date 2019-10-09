---
draft: false
menu:
  fall2019:
    name: Logistics
summary: "CSCI 5576/4576: Fall 2019: High Performance Scientific Computing"
title: Logistics
toc: true
type: docs
weight: 1
---

## Where and When

* CSCI 5576 / 4576: High Performance Scientific Computing
* Lectures: **Mon/Wed/Fri 11-11:50** in ECCR 150
* Labs: **Fri 9-10:40** in KOBL 300
* Website: https://cucs-hpsc.github.io/fall2019
* Issues/Questions/etc.: [Browse](https://github.com/cucs-hpsc/hpsc-class/issues) or [Create](https://github.com/cucs-hpsc/hpsc-class/issues/new)

#### Zoom
- Meeting ID: 214-104-523
- Join via web browser: https://cuboulder.zoom.us/j/214104523
- Join via Zoom app (using meeting ID)
- Join via One tap mobile: +16699006833,,214104523# or +16465588656,,214104523#
- Join via telephone: 1-669-900-6833 or 1-646-558-8656

#### Calendar

<iframe src="https://calendar.google.com/calendar/embed?src=0vto3a2spansbnhgb0jcm5a16c%40group.calendar.google.com&ctz=America%2FDenver" style="border: 0" width="800" height="600" frameborder="0" scrolling="no"></iframe>

#### Instructor: [Jed Brown](https://www.colorado.edu/cs/jed-brown)
* GitHub: [@jedbrown](https://github.com/jedbrown)
* Office hours: See calendar in ECOT 824 (usually Tue 14:30-15:30 and Thu 9:00-10:00)

#### Teaching Assistant: [Camden Elliott-Williams](https://www.colorado.edu/ics/camden-elliott-williams)
* GitHub: [@CamdenCU](https://github.com/CamdenCU)
* Office hours: Wed 9:30-10:30 and 13:30-14:30 or by appointment in ECCR 1B "Systems Lab" ([see map](/img/camden-desk.png))

## Homework

For each assignment, click the link below to accept via GitHub
Classroom.  This creates a private repository for you to work in.
Then `git clone` the repository to whatever machine you'll work on and
follow instructions in the README.  Usually you will be asked to read
and edit code, run a range of experiments, and interpret/plot data in
a `Report.ipynb`.

| Assigned | Due | Description |
|----------|-----|-------------|
| 2019-09-06 | 2019-09-16 (part by 2019-09-13) | [Experiments in vectorization](https://classroom.github.com/a/DOC_yYu2)
| 2019-09-20 | 2019-09-30 | [Sorting](https://classroom.github.com/a/d4_gpzck) |

## Videos

Videos appear automatically on
[Canvas](https://canvas.colorado.edu/courses/54520/external_tools/2040)
and linked below.

| Date | Topic |
|------|-------|
| [Aug 26](https://cu-classcapture.colorado.edu/Mediasite/Play/055148f7c5c141578bb467d237da1d801d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Course introduction and preview of architectural trends |
| [Aug 28](https://cu-classcapture.colorado.edu/Mediasite/Play/e6af2695fcfe4276b08a3e85ea625dbf1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Intro to architecture |
| [Aug 30](https://cu-classcapture.colorado.edu/Mediasite/Play/f062611e09f64437a13f64de570bc30e1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Intro to vectorization and ILP |
| [Sep 4](https://cu-classcapture.colorado.edu/Mediasite/Play/422cf46ffd1844bb8492e8831f92d9461d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Intro to performance modeling (roofline) |
| [Sep 6](https://cu-classcapture.colorado.edu/Mediasite/Play/6446d91e5f2f46b680d4ce5492e88f9b1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Intro to parallel scaling |
| [Sep 9](https://cu-classcapture.colorado.edu/Mediasite/Play/e4b2187d5d2d45dca8321a86e0e0e8a41d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Joel Frahm on CU [Research Computing](https://www.colorado.edu/rc/) ([slides](https://github.com/ResearchComputing/CSCI-5576/blob/master/RC-CSCI-5576.pdf)) |
| [Sep 11](https://cu-classcapture.colorado.edu/Mediasite/Play/ef164dcd135b487ca938617d8823340c1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | OpenMP Basics |
| [Sep 13](https://cu-classcapture.colorado.edu/Mediasite/Play/39793bba0c824472a92202a3816858b91d?catalog=38dea75ea63b491fa8cc2804c084abb821) | OpenMP memory semantics, synchronization, and `perf` demo |
| [Sep 16](https://cu-classcapture.colorado.edu/Mediasite/Play/51f737b31b4a4f13a6082e4dcd1adedd1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | OpenMP tasking and computational depth/critical path |
| [Sep 18](https://cu-classcapture.colorado.edu/Mediasite/Play/b7c3da0053d54ab99e4884a5ce7c93e51d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Low-level optimization, parallel reductions and scans |
| [Sep 20](https://cu-classcapture.colorado.edu/Mediasite/Play/4233af0b8ef54477bf63590c2229a26f1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Searching and sorting methods (based on parts of [slides](http://home.cs.colorado.edu/~rahimian/articles/18/hpc18-03-search.sl.html) and [slides](http://home.cs.colorado.edu/~rahimian/articles/18/hpc18-03-tech.sl.html)) |
| [Sep 23](https://cu-classcapture.colorado.edu/Mediasite/Play/e85296370c80494492649d73fc068f111d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Bitonic sort recap/demo; intro to graph independence |
| [Sep 25](https://www.youtube.com/watch?v=8bB2L1-8rBI) | Recorded lecture: Introduction to MPI |
| [Sep 27](https://cu-classcapture.colorado.edu/Mediasite/Play/6551873dad33443087afc7d4339a28141d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Library interfaces with MPI: Conway's Game of Life |
| [Sep 30](https://cu-classcapture.colorado.edu/Mediasite/Play/0fccb0cac51145d694f95a3b7c4e043a1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Dense linear algebra and networks |
| [Oct 2](https://cu-classcapture.colorado.edu/Mediasite/Play/fc99deffabad45899dfc6bfab7099a201d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Dense linear algebra and orthogonality |
| [Oct 4](https://cu-classcapture.colorado.edu/Mediasite/Play/34ae8e391dbb4ce0b9f859837d8a4bdf1d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Orthogonality and conditioning |
| [Oct 7](https://cu-classcapture.colorado.edu/Mediasite/Play/6ab776e8c8094589894f5a1cfd6cb6f51d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Parallel QR and Elemental for distributed memory |
| [Oct 9](https://cu-classcapture.colorado.edu/Mediasite/Play/e02c02c5f9894a418e02b4c310129d021d?catalog=38dea75ea63b491fa8cc2804c084abb821) | Sparse and iterative linear algebra |
