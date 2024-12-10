#define PIXELS_PER_TILE 16
#include <math.h>
#include <stdlib.h>
#include "engine/logic/bbTerminal.h"

extern double* lanczos_kernel;



double lanczos(double x);

I32 gen_lanczos_kernel(void);
I32 gen_gaussdiff_kernel(void);
I32 export_lanczos_kernel();
I32 export_gaussdiff_kernel();