#include <APSP_ADT.h>
#include <stdlib.h>
#include <stdio.h>
#include <floyd_warshal.h> //Lo incluye solo por la funcion abs_malloc()
//#include <test_graphs.h>
#include <graph_basics.h>

//Reserva en memoria espacio para el grafo de distancias y el de caminos
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

//Libera la memoria
//Public
void APSP_graph_destroy(t_APSP_graph* G){
    abs_free(G->D);
    #ifndef NO_PATH
		abs_free(G->P);
	#endif
}

//Inicializa el grafo de reconstruccion de caminos minimos, ordenado por filas (estandar).
//Private
static void initNotBlockedPathGraph(t_APSP_graph* G){
	for(INT64 i=0; i<G->n; i++)
		for(INT64 j=0; j<G->n; j++)
		    if(G->D[i*G->n+j] != INFINITE)
    			G->P[i*G->n+j] = j;
    		else
    			G->P[i*G->n+j] = -1;
}

//Inicializa el grafo de reconstruccion de caminos minimos, ordenado por bloques.
//Private
static void initBlockedPathGraph(t_APSP_graph* G){
	INT64 I,J,i,j,blockSize,r,idx;
	r = G->n/BS;
	blockSize = BS*BS; //blockSize = Block size total, es decir, la cantidad de elementos totales que contiene un bloque teniendo en cuenta sus dos dimensiones
	for(I=0; I<r; I++){
		for(J=0; J<r; J++){
			for(i=0; i<BS; i++){
				for(j=0; j<BS; j++){
				    //I*n*BS = offset de toda la fila I de bloques
				    //J*blockSize = offset del bloque J dentro de la fila de bloques I
				    //i*BS = offset de la fila i dentro del bloque J de la fila de bloques I
				    //j = elemento dentro de la fila i del bloque J de la fila de bloques I
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
/*
//Public
void APSP_import_distance_graph(t_APSP_graph* G, TYPE* inputGraph){
	if(G->bs == 0){
		copyTYPEGraph(G->D, inputGraph, G->n);
	} else {
		TYPEGraphFromRowsToBlocks(G->D, inputGraph, G->n, G->bs);
	}
}
*/
//Public
void APSP_init_path_graph(t_APSP_graph* G){
	if(G->implementsBlocking){
		initBlockedPathGraph(G);
	} else {
		initNotBlockedPathGraph(G);
	}
}

//Inicializa el grafo de distancias y el de caminos: Copia inputGraph en G->D e inicializa G->P.
//G puede estar o no ordenado por bloques. inputGraph debe estar ordenado por filas (estandar).
//Public
/*
void APSP_graph_init(t_APSP_graph* G, TYPE* inputGraph){
	initDistanceGraph(G, inputGraph);
	initPathGraph(G);
}
*/
//Compara los grafos de 'A' con los de 'B'. Si son iguales entonces retorna 0, sino retorna 1, 2 o 3 segun cuales son los grafos que presentan diferencias (ver en el codigo).
//'A' puede estar ordenado por bloques, pero 'B' NO.
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
