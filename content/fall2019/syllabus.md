---
title: Syllabus
toc: true
type: docs
weight: 1
menu:
  fall2019:
    name: Syllabus
---

### Overview

This course will develop the skills necessary to reason about
performance of applications and modern architectures, to identify
opportunities and side-effects of changes, to develop high-performance
software, to transfer algorithmic patterns and lessons learned from
different domains, and to communicate such analyses with diverse
stakeholders.  These skills are important for research and development
of numerical methods and performance-sensitive science and engineering
applications as well as jobs affiliated with computing facilities in
academia, [national labs](https://www.alcf.anl.gov/about/careers), and
industry.

We will introduce widely-used parallel programming models such as
OpenMP, MPI, and CUDA, as well as ubiquitous parallel libraries, but
the purpose of the course is not to teach interfaces, but to develop
skills that will be durable and transferrable.

### Preparation

This course does not assume prior experience with parallel
programming.  It will use Linux command-line tools, and some
activities will involve batch computing environments (SLURM).  Most
exercises will use the C programming language, though you can use any
appropriate language for projects.  Some of the exercises will involve
techniques from numerical computing (e.g., CSCI-3656).  I will do my
best to avoid assuming prior knowledge of these topics, and to provide
resources for you to learn or refresh your memory as we use them.

Everyone here is capable of succeeding in the course, but the effort
level will be higher if most of the topics above are new to you.
Regardless of your preparation, it is normal to feel lost sometimes.
A big part of pragmatic HPC is learning to efficiently answer your
questions through documentation, online resources, and even consulting
the code or running experiments.  (Most of our software stack is open
source.)  That said, it's easy to lose lots of time in a rabbit hole.
My hope is that you will have the courage to dive into that rabbit
hole occasionally, but also to ask questions when stuck and to budget
your time for such excursions so that you can complete assignments
on-time without compromising your work/life balance.

### Approximate timeline

| Week   | Topics |
|--------|--------|
| Aug 26 | Introduction and modern computer architecture (vectorization and memory hierarchy) |
| Sep 2  | Performance modeling, analysis, and scaling; profiling |
| Sep 9  | Intro to OpenMP and non-numerical algorithms (sorting and searching) |
| Sep 16 | Parallel algorithmic patterns |
| Sep 23 | Dense linear algebra |
| Sep 30 | Intro to MPI and distributed memory parallelism |
| Oct 7  | Sparse and iterative linear algebra |
| Oct 14 | Domain decomposition |
| Oct 21 | Graph algorithms |
| Oct 28 | GPU programming via OpenMP-5 and CUDA |
| Nov 4  | Parallel file systems and IO |
| Nov 11 | Data analysis/machine learning algorithms |
| Nov 18 | Particles and N-body systems |
| Nov 25 | Fall Break |
| Dec 2  | Multigrid, FFT, and FMM |
| Dec 9  | Special topics |

### Evaluation

| Activity  | Percentage |
|-----------|------------|
| Participation | 10% |
| Labs and homework assignments | 40% |
| Community contribution | 15% |
| Community analysis | 15% |
| Final project (written + presentation) | 20% |

#### Git and GitHub

Homework assignments and in-class activities will be submitted via Git.  This class will use GitHub classroom.
Homeworks will be completed by cloning GitHub repositories, completing coding and analysis activities, and pushing completed assignments back to GitHub.

Assignments may be completed using [Coding CSEL Hub](https://coding.csel.io/) and/or [RMACC Summit](https://www.colorado.edu/rc/resources/summit/specifications) ([request an account](https://rcamp.rc.colorado.edu/accounts/account-request/create/organization)).
Assignments will typically have written analysis, for which I recommend [Jupyter](https://jupyter.org/).

It is notoriously difficult to predict the time required to develop
quality code and understand its performance, so please start early to
give yourself plenty of time.  You are welcome to work together on all
assignments, but must give credit to collaborators.  You should ensure
that your written work is entirely your own.

### Moodle

Moodle will be used to maintain grades.  Please enroll yourself at https://moodle.cs.colorado.edu.

### Resources

This course will use a variety of online resources and papers.
There is no required textbook, but the following resources may be helpful.

* [Hager and Wellein (2010), **Introduction to High Performance Computing for Scientists and Engineers**](https://blogs.fau.de/hager/hpc-book)
* [van de Geijn, Myers, Parikh (2019): **LAFF on Programming for High Performance**](http://www.cs.utexas.edu/users/flame/laff/pfhp/index.html) (free online)
* [Eijkhout (2017), **Introduction to High-Performance Scientific Computing**](http://pages.tacc.utexas.edu/~eijkhout/istc/istc.html) (free PDF)

#### Additional resources

* [Greenbaum and Chartier (2012), **Numerical Methods Design, Analysis, and Computer Implementation of Algorithms**](https://press.princeton.edu/titles/9763.html) -- an excellent, comprehensive book.
* [Boyd and Vandenberghe (2018), **Introduction to Applied Linear Algebra**](https://web.stanford.edu/~boyd/vmls/) -- practical introduction to linear algebra for computer scientists; free PDF
* [Trefethen and Bau (1997), **Numerical Linear Algebra**](http://bookstore.siam.org/ot50/) -- fantastic, but limited to numerical linear algebra and covers more advanced topics.
* [Scopatz and Huff (2015), **Effective Computation in Physics**](http://physics.codes/) -- Python language, data science workflow, and computation.

A [SIAM Membership](http://www.siam.org/students/memberships.php) is free for CU students and provides a 30% discount on SIAM books.

### Disability Accommodations

If you qualify for accommodations because of a disability, please submit to your professor a letter from Disability Services in a timely manner (for exam accommodations provide your letter at least one week prior to the exam) so that your needs can be addressed. Disability Services determines accommodations based on documented disabilities. Contact Disability Services at 303-492-8671 or by e-mail at dsinfo@colorado.edu. If you have a temporary medical condition or injury, see the Temporary Injuries guidelines under the Quick Links at the Disability Services website and discuss your needs with your professor.

### Religious Observances

[Campus policy regarding religious observances](http://www.colorado.edu/policies/fac_relig.html) requires that faculty make every effort to deal reasonably and fairly with all students who, because of religious obligations, have conflicts with scheduled exams, assignments or required assignments/attendance. If this applies to you, please speak with me directly as soon as possible at the beginning of the term. See the [campus policy regarding religious observances](http://www.colorado.edu/policies/observance-religious-holidays-and-absences-classes-andor-exams) for full details.

### Classroom Behavior

Students and faculty each have responsibility for maintaining an appropriate learning environment. Those who fail to adhere to such behavioral standards may be subject to discipline. Professional courtesy and sensitivity are especially important with respect to individuals and topics dealing with differences of race, color, culture, religion, creed, politics, veteran's status, sexual orientation, gender, gender identity and gender expression, age, disability,and nationalities. Class rosters are provided to the instructor with the student's legal name. I will gladly honor your request to address you by an alternate name or gender pronoun. Please advise me of this preference early in the semester so that I may make appropriate changes to my records. For more information, see the policies on [classroom behavior](http://www.colorado.edu/policies/student-classroom-and-course-related-behavior) and the [student code](http://www.colorado.edu/osc/sites/default/files/attached-files/studentconductcode_16-17-a.pdf).

### Discrimination and Harassment

The University of Colorado Boulder (CU Boulder) is committed to maintaining a positive learning, working, and living environment. CU Boulder will not tolerate acts of sexual misconduct, discrimination, harassment or related retaliation against or by any employee or student. CU's Sexual Misconduct Policy prohibits sexual assault, sexual exploitation, sexual harassment,intimate partner abuse (dating or domestic violence), stalking or related retaliation. CU Boulder's Discrimination and Harassment Policy prohibits discrimination, harassment or related retaliation based on race, color,national origin, sex, pregnancy, age, disability, creed, religion, sexual orientation, gender identity, gender expression, veteran status, political affiliation or political philosophy. Individuals who believe they have been subject to misconduct under either policy should contact the Office of Institutional Equity and Compliance (OIEC) at 303-492-2127. Information about the OIEC, the above referenced policies, and the campus resources available to assist individuals regarding sexual misconduct, discrimination, harassment or related retaliation can be found at the [OIEC website](http://www.colorado.edu/institutionalequity/).

### Honor Code

All students enrolled in a University of Colorado Boulder course are responsible for knowing and adhering to the [academic integrity policy](http://www.colorado.edu/policies/academic-integrity-policy) of the institution. Violations of the policy may include: plagiarism, cheating,fabrication, lying, bribery, threat, unauthorized access, clicker fraud,resubmission, and aiding academic dishonesty.  All incidents of academic misconduct will be reported to the Honor Code Council (honor@colorado.edu; 303-735-2273). Students who are found responsible for violating the academic integrity policy will be subject to nonacademic sanctions from the Honor Code Council as well as academic sanctions from the faculty member. Additional information regarding the academic integrity policy can be found at http://honorcode.colorado.edu.


