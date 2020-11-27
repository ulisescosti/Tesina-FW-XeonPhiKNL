#include <APSP_ADT.h>
#include <stdlib.h>
#include <stdio.h>
#include <floyd_warshal.h> //This file includes it just to use the function abs_malloc()
#include <graph_basics.h>

//Allocs memory for D and P graphs
//Public
void APSP_graph_new(t_APSP_graph* G, int n, int implementsBlocking){
	G->D = (TYPE*)abs_malloc((INT64)n*(INT64)n*sizeof(TYPE));
	G->implementsBlocking = implementsBlocking;
	#ifdef NO_PATH
		G->P = NULL;
	#else
		G->P = (int*)abs_malloc((INT64)n*(INT64)n*sizeof(int));
	#endif
	G->n = n;
}

//Memory free
//Public
void APSP_graph_destroy(t_APSP_graph* G){
    abs_free(G->D);
    #ifndef NO_PATH
		abs_free(G->P);
	#endif
}

//It initializes the P graph, ordered by rows (standard)
//Private
static void initNotBlockedPathGraph(t_APSP_graph* G){
	for(INT64 i=0; i<G->n; i++)
		for(INT64 j=0; j<G->n; j++)
		    if(G->D[i*G->n+j] != INFINITE)
    			G->P[i*G->n+j] = j;
    		else
    			G->P[i*G->n+j] = -1;
}

//It initializes the P graph, ordered by blocks
//Private
static void initBlockedPathGraph(t_APSP_graph* G){
	INT64 I,J,i,j,blockSize,r,idx;
	r = G->n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++){
		for(J=0; J<r; J++){
			for(i=0; i<BS; i++){
				for(j=0; j<BS; j++){
				    //I*n*BS = offset of the entire row of blocks number I
				    //J*blockSize = offset of block J inside the row of blocks I
				    //i*BS = offset of row i inside the block J of the row of blocks I
				    //j = position inside the row i of the block J of the row of blocks I
					idx = I*G->n*BS+J*blockSize+i*BS+j;					
					if(G->D[idx] != INFINITE)
						G->P[idx] = J*BS+j;
					else
						G->P[idx] = -1;
				}
			}
		}
	}
}

//Public
void APSP_init_path_graph(t_APSP_graph* G){
	if(G->implementsBlocking){
		initBlockedPathGraph(G);
	} else {
		initNotBlockedPathGraph(G);
	}
}

//It compares the graphs of 'A' and 'B'. If they are equal, it returns 0, else, it returns 1, 2 or 3, depending on which graphs have diferences.
//'A' could be ordered by blocks, but not 'B'.
//Public
int APSP_graph_equals(t_APSP_graph* A, t_APSP_graph* B){
	int distanceGraphOK;
	int pathGraphOK;

	if(A->implementsBlocking){
		distanceGraphOK = TYPEGraphsWBlocksEquals(A->D, B->D, A->n);
		#ifdef NO_PATH
			pathGraphOK = 1; //True
		#else
			pathGraphOK = intGraphsWBlocksEquals(A->P, B->P, A->n, B->D);
		#endif
	} else {
		distanceGraphOK = TYPEGraphsEquals(A->D, B->D, A->n);
		#ifdef NO_PATH
			pathGraphOK = 1; //True		
		#else
			pathGraphOK = intGraphsEquals(A->P, B->P, A->n);
		#endif
	}
	if(distanceGraphOK){
		if(pathGraphOK){
			return 0;
		} else {
			return 1;
		}
	} else {
		if(pathGraphOK){
			return 2;
		} else {
			return 3;
		}
	}
}
