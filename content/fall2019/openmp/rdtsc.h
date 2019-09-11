#ifndef rdtsc_h
#define rdtsc_h

// The RDTSC instruction is a high-resolution way to access a nominal cycle (or
// "tick") counter.  It's often higher resolution than clock_gettime() or
// gettimeofday(), which measure in seconds.  It is only available on x86 and
// the precise meaning is architecture-dependent, so it is not a good choice if
// you need a portable timer.
//
// https://en.wikipedia.org/wiki/Time_Stamp_Counter
//
// Note that CPU clock rates are dynamically throttled to conserve power (low
// utilization) and to avoid overheating (high utilization) so you may see
// execution speed vary.

typedef unsigned long long ticks_t;
ticks_t rdtsc() {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ((ticks_t)lo)|( ((ticks_t)hi)<<32);
}

#endif
