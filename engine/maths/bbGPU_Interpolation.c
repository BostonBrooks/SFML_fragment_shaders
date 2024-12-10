#define PIXELS_PER_TILE 16
#include <math.h>
#include <stdlib.h>
#include "engine/logic/bbTerminal.h"

double* lanczos_kernel;
double* gaussdiff_kernel;



double lanczos(double x){
	//a = 2; -2 <= x <= 2
	if (x < 0.01 && x>-0.01){

		return (
				1.0
				- 5.0 * M_PI * M_PI * x * x / 24.0
				+ 91 * M_PI * M_PI * M_PI * M_PI * x * x * x * x / 5760.0
		);
	}

	if (x>= 2 || x <= -2) return 0;

	return 2*sin(x * M_PI) * sin( x * M_PI/2.0) / (M_PI * M_PI * x * x);

}

I32 gen_lanczos_kernel(void){
	lanczos_kernel = calloc(4*PIXELS_PER_TILE+1,sizeof (double));

	bbAssert(lanczos_kernel != NULL, "bad malloc\n");

	printf("Lanczos\n");
	double x;
	for (I32 i = 0; i < 4*PIXELS_PER_TILE+1; i++){
		x = - 2.0 + (double)i /  PIXELS_PER_TILE;
		double y = lanczos(x);
		lanczos_kernel[i] = y;
		printf("%lf, %lf\n",x, y);
	}
	printf("\n");

	return Success;
}

double gaussdiff(double x){
	return exp(-x*x)*x;
}

I32 gen_gaussdiff_kernel(void) {
	gaussdiff_kernel = calloc(10 * PIXELS_PER_TILE + 1, sizeof(double));

	bbAssert(gaussdiff_kernel != NULL, "bad malloc\n");

	printf("Gaussian Differential\n");
	double x;
	for (I32 i = 0; i < 6 * PIXELS_PER_TILE + 1; i++) {
		x = -3.0 + (double) i / PIXELS_PER_TILE;
		double y = gaussdiff(x);
		gaussdiff_kernel[i] = y;
		printf("%lf, %lf\n",x, y);
	}
	printf("\n");

	return Success;
}

I32 export_lanczos_kernel(){
	printf("    const float[] L_Kernel = {");
	for (I32 i = 0; i < 4*PIXELS_PER_TILE; i++){
		printf("%lf, ", lanczos_kernel[i]);
	}
	printf("%lf};\\\n", lanczos_kernel[4*PIXELS_PER_TILE]);
}

I32 export_gaussdiff_kernel(){
	printf("    const float[] GD_Kernel = {");
	for (I32 i = 0; i < 6*PIXELS_PER_TILE; i++){
		printf("%lf, ", gaussdiff_kernel[i]);
	}
	printf("%lf};\\\n", gaussdiff_kernel[3*PIXELS_PER_TILE]);
}