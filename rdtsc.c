double irp_rdtsc_(void) {
  unsigned long long a, d;
  __asm__ volatile ("rdtscp" : "=a" (a), "=d" (d));
  return (double)((d<<32) + a);
}
