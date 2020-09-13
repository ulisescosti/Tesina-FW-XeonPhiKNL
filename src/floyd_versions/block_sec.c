#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/malloc/no_align.c"


//Public
char* getFloydName(){
	return "secuencial con bloques";
}

//Public
char* getFloydVersion(){
	return "Block-Sec";
}

//Public
int versionImplementsBlocking(){
	return 1; //True
}

//Public
int versionIsParallel(){
	return 0; //False
}


static inline void FW_BLOCK(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(TYPE* const graph, const INT64 d1, const INT64 d2, const INT64 d3, int* const path, const INT64 base){
	INT64 i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
	TYPE dij, dik, dkj, sum;

	for(k=0; k<BS; k++){ //This BS is a const. Not the BS argument.
		k_disp = k*BS;
		k_disp_d3 = k_disp + d3;
		for(i=0; i<BS; i++){
			i_disp = i*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph[i_disp + d2 + k];
			for(j=0; j<BS; j++){
				dij = graph[i_disp_d1 + j];
				dkj = graph[k_disp_d3 + j];
				sum = dik + dkj;
				if(sum < dij){
					graph[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#endif
				}
			}
		}
	}
}

//Public
#pragma GCC diagnostic ignored "-Wunused-parameter"
void floydWarshall(TYPE* D, int* P, int n, int t){ //t es ignorado, solo se mantiene para "estandarizar" los argumentos de todas las funciones floyd
	INT64 i, j, k, r, b, kj, ik, kk, ij, row_of_blocks_disp, block_size, k_row_disp, k_col_disp, i_row_disp, j_col_disp;
	
	r = n/BS;
	row_of_blocks_disp = n*BS;
	block_size = BS*BS;
	
	for(k=0; k<r; k++){
		b = k*BS;
		k_row_disp = k*row_of_blocks_disp;
		k_col_disp = k*block_size;
		
		//Fase 1
		kk = k_row_disp + k_col_disp;
		FW_BLOCK(D, kk, kk, kk, P, b);
		
		//Fase 2
		for(j=0; j<r; j++){
			if(j == k)
		        continue;
			kj = k_row_disp + j*block_size;
			FW_BLOCK(D, kj, kk, kj, P, b);
		}
		
		//Fase 3
		for(i=0; i<r; i++){
			if(i == k)
		        continue;
			ik = i*row_of_blocks_disp + k_col_disp;
			FW_BLOCK(D, ik, ik, kk, P, b);
		}
		
		//Fase 4
		for(i=0; i<r; i++){
		    if(i == k)
				continue;
			i_row_disp = i*row_of_blocks_disp;
			ik = i_row_disp + k_col_disp;
			for(j=0; j<r; j++){
				if(j == k)
					continue;
				j_col_disp = j*block_size;
				kj = k_row_disp + j_col_disp;
				ij = i_row_disp + j_col_disp;
				FW_BLOCK(D, ij, ik, kj, P, b);
			}
		}
	}
}
