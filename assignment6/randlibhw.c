#include <immintrin.h>
#include "randlib.h"

/* Initialize the hardware rand64 implementation.  */
// static void
// hardware_rand64_init(void)
// {
// }

/* Return a random value, using hardware operations.  */
extern unsigned long long
rand64(void)
{
  unsigned long long int x;
  while (!_rdrand64_step(&x))
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */
// static void
// hardware_rand64_fini(void)
// {
// }