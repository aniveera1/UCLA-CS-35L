#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__ ((__constructor__))
static void software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Finalize the software rand64 implementation.  */
__attribute__ ((__destructor__))
static void software_rand64_fini (void)
{
  fclose (urandstream);
}

extern unsigned long long rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}
