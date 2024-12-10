#include "../engine/maths/bbGPU_Interpolation.h"



int main(void){

	gen_lanczos_kernel();
	gen_gaussdiff_kernel();
	export_lanczos_kernel();
	export_gaussdiff_kernel();

}