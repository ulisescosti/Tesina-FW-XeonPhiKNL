#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/malloc/no_align.c"
#include <omp.h>


//Public
char* getFloydName(){
	return "Naive-Par";
}

//Public
char* getFloydVersion(){
	return "paralelo \"naive\"";
}

//Public
int versionImplementsBlocking(){
	return 0; //False
}

//Public
int versionIsParallel(){
	return 1; //True
}

//Public
#pragma GCC diagnostic ignored "-Wunused-parameter"
void floydWarshall(TYPE* D, int* P, int n, int t){
	#pragma omp parallel default(none) firstprivate(D,P,n) num_threads(t)
	{
		INT64 i, j, k, i_disp, k_disp;
		TYPE dij, dik, dkj, sum;
		for(k=0; k<n; k++){
			k_disp = k*n;
			#pragma omp for schedule(dynamic)
			for(i=0; i<n; i++){
				i_disp = i*n;
				dik = D[i_disp+k];
				for(j=0; j<n; j++){
					dij = D[i_disp+j];
					dkj = D[k_disp+j];
					sum = dik + dkj;
					if(sum < dij){
						D[i_disp+j] = sum;
						#ifndef NO_PATH
							P[i_disp+j] = k;
						#endif
					}
				}
			}
		}
	}
}
