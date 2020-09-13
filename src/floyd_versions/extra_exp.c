#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_0-n.c"
#include "../src/floyd_versions/common/malloc/aligned.c"

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define UNROLL_FACTOR BS/(SIMD_WIDTH/TYPE_SIZE)

//Public
char* getFloydName(){
	return "experimento extra";
}

//Public
char* getFloydVersion(){
	return "Extra-Exp";
}
/*
//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(TYPE* const D1, TYPE* const D2, TYPE* const D3, int* const path, const INT64 BS, const INT64 base){
    INT64 i, j, k, i_disp, k_disp;
    TYPE dij, dik, dkj, sum;
    
	for(k=0; k<BS; k++){
		k_disp = k*BS;
		for(i=0; i<BS; i++){
			i_disp = i*BS;
			dik = D2[i_disp + k];
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#else
				#pragma GCC unroll UNROLL_FACTOR
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = D1[i_disp + j];
				dkj = D3[k_disp + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
                    D1[i_disp + j] = sum;
                    #ifndef NO_PATH
                    	path[i_disp + j] = base + k;
                    #endif
                }
			}
		}
	}
}

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(TYPE* const D1, TYPE* const D2, TYPE* const D3, int* const path, const INT64 BS, const INT64 base){
    INT64 i, j, k, i_disp, k_disp;
    TYPE dij, dik, dkj, sum;
    
	for(k=0; k<BS; k++){
		k_disp = k*BS;
		#pragma omp for
		for(i=0; i<BS; i++){
			i_disp = i*BS;
			dik = D2[i_disp + k];			
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#else
				#pragma GCC unroll UNROLL_FACTOR
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = D1[i_disp + j];
				dkj = D3[k_disp + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
                    D1[i_disp + j] = sum;
                    #ifndef NO_PATH
                	    path[i_disp + j] = base + k;
                    #endif
                }
			}
		}
	}
}
*/
static inline void FW_BLOCK(TYPE* const D1, TYPE* const D2, TYPE* const D3, int* const path, const INT64 base, int* const tmp1, int* const tmp2) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(TYPE* const D1, TYPE* const D2, TYPE* const D3, int* const path, const INT64 base, int* const tmp1, int* const tmp2){
    INT64 i, j, k, i_disp, k_disp;
    TYPE dij, dik, dkj, sum;
    
	for(k=0; k<BS; k++){
		k_disp = k*BS;
		for(i=0; i<BS; i+=2){
			i_disp = i*BS;
			dik = D2[i_disp + k];
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = D1[i_disp + j];
				dkj = D3[k_disp + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
                    D1[i_disp + j] = sum;
                    #ifndef NO_PATH
                    	path[i_disp + j] = base + k;
                    #else
	                    tmp1[j] = tmp2[j];
	                #endif
                }
			}
			i_disp = (i+1)*BS;
			dik = D2[i_disp + k];
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = D1[i_disp + j];
				dkj = D3[k_disp + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
                    D1[i_disp + j] = sum;
                    #ifndef NO_PATH
                    	path[i_disp + j] = base + k;
                    #else
	                    tmp1[j] = tmp2[j];
	                #endif
                }
			}
		}
	}
}

static inline void FW_BLOCK_PARALLEL(TYPE* const D1, TYPE* const D2, TYPE* const D3, int* const path, const INT64 base, int* const tmp1, int* const tmp2) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(TYPE* const D1, TYPE* const D2, TYPE* const D3, int* const path, const INT64 base, int* const tmp1, int* const tmp2){
    INT64 i, j, k, i_disp, k_disp;
    TYPE dij, dik, dkj, sum;
    
	for(k=0; k<BS; k++){
		k_disp = k*BS;
		#pragma omp for
		for(i=0; i<BS; i+=2){
			i_disp = i*BS;
			dik = D2[i_disp + k];			
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = D1[i_disp + j];
				dkj = D3[k_disp + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
                    D1[i_disp + j] = sum;
                    #ifndef NO_PATH
                	    path[i_disp + j] = base + k;
                    #else
	                    tmp1[j] = tmp2[j];
	                #endif
                }
			}
			i_disp = (i+1)*BS;
			dik = D2[i_disp + k];			
			#ifdef INTEL_ARC
				#pragma unroll(UNROLL_FACTOR)
			#endif
			#pragma omp simd private(dij,dkj,sum)
			for(j=0; j<BS; j++){
				dij = D1[i_disp + j];
				dkj = D3[k_disp + j];
				sum = dik + dkj;
				if(unlikely(sum < dij)){
                    D1[i_disp + j] = sum;
                    #ifndef NO_PATH
                	    path[i_disp + j] = base + k;
                    #else
	                    tmp1[j] = tmp2[j];
	                #endif
                }
			}
		}
	}
}

//Public
void floydWarshall(TYPE* D, int* P, int n, int t){
	INT64 r, row_of_blocks_disp, block_size;
	r = n/BS;
	row_of_blocks_disp = n*BS;
	block_size = BS*BS;
	#pragma omp parallel default(none) firstprivate(r,row_of_blocks_disp,block_size,D,P) num_threads(t)
	{
		INT64 i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w;
		int tmp1[BS], tmp2[BS];
		for(k=0; k<r; k++){
			b = k*BS;
			k_row_disp = k*row_of_blocks_disp;
			k_col_disp = k*block_size;

			//Fase 1
			kk = k_row_disp + k_col_disp;
			FW_BLOCK_PARALLEL(D+kk, D+kk, D+kk, P+kk, b, tmp1, tmp2);

			//Fase 2 y 3
			#pragma omp for schedule(dynamic)
			for(w=0; w<r*2; w++){
				if(w<r){ //Fase 2
					j = w;
					if(j == k)
						continue;
					kj = k_row_disp + j*block_size;
					FW_BLOCK(D+kj, D+kk, D+kj, P+kj, b, tmp1, tmp2);
				} else { //Fase 3
					i = w - r;
					if(i == k)
						continue;
					ik = i*row_of_blocks_disp + k_col_disp;
					FW_BLOCK(D+ik, D+ik, D+kk, P+ik, b, tmp1, tmp2);
				}
			} 
	/*		//Fase 2
			#pragma omp for nowait
			for(j=0; j<r; j++){
				if(j == k)
					continue;
				kj = k_row_disp + j*block_size;
				FW_BLOCK(D+kj, D+kk, D+kj, P+kj, BS, b);
			} 
			//Fase 3
			#pragma omp for
			for(i=0; i<r; i++){
				if(i == k)
					continue;
				ik = i*row_of_blocks_disp + k_col_disp;
				FW_BLOCK(D+ik, D+ik, D+kk, P+ik, BS, b);
			}*/

			//Fase 4
			#pragma omp for collapse(2) schedule(dynamic)
			for(i=0; i<r; i++){
				for(j=0; j<r; j++){
					if( (j == k) || (i == k) )
						continue;
					i_row_disp = i*row_of_blocks_disp;
					ik = i_row_disp + k_col_disp;
					j_col_disp = j*block_size;
					kj = k_row_disp + j_col_disp;
					ij = i_row_disp + j_col_disp;
					FW_BLOCK(D+ij, D+ik, D+kj, P+ij, b, tmp1, tmp2);
				}
			}
		}
	}
}
