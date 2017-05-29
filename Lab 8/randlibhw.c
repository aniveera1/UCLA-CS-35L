#include "randlib.h"
#include <immintrin.h>

/* Return a random value, using hardware operations.  */
extern unsigned long long rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}
