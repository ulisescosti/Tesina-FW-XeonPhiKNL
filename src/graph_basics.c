#include <graph_basics.h>
#include <stdio.h>
#include <stdlib.h>

//Comparacion plana. Grafos de tipo TYPE.
//Public
int TYPEGraphsEquals(TYPE* A, TYPE* B, int n){
	int vectorLength = n*n;
	for(int i=0; i<vectorLength; i++)
		if(A[i] != B[i])
			return 0;
	return 1;
}

//Comparacion plana. Grafos de tipo int.
//Public
int intGraphsEquals(int* A, int* B, int n){
	int vectorLength = n*n;
	for(int i=0; i<vectorLength; i++)
		if(A[i] != B[i])
			return 0;
	return 1;
}

//Comparacion custom. A ordenado por bloques, B ordenado por filas (estandar). Grafos de tipo TYPE.
//Public
int TYPEGraphsWBlocksEquals(TYPE* A, TYPE* B, int n){ 
	int I,J,i,j,blockSize,r;
	r = n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<BS; i++)
				for(j=0; j<BS; j++)
					if(A[I*n*BS+J*blockSize+i*BS+j] != B[I*n*BS+J*BS+i*n+j]){
			/*			printf("A[%d]= %2.0f != B[%d]= %2.0f\n", I*n*BS+J*blockSize+i*BS+j, A[I*n*BS+J*blockSize+i*BS+j], I*n*BS+J*BS+i*n+j, B[I*n*BS+J*BS+i*n+j]);
						exit(0);*/
						return 0;
					}
						
	return 1;
}

typedef struct{
	int I;
	int J;
	int i;
	int j;
} CompositeIndex;

//Private
static void breakDownIndex(CompositeIndex* c, int idx, int n){
	int x, blk_row_disp, blockSize;
	blockSize = BS*BS;
	blk_row_disp = n*BS;
	
	x = idx;
	c->I = x/blk_row_disp;
	x = x%blk_row_disp;
	
	c->J = x/blockSize;
	x = x%blockSize;
	
	c->i = x/BS;
	x = x%BS;
	
	c->j = x;
}

typedef struct{
	int X;
	int x;
} VertexIndexes;

//Private
static void getVertexIndexes(VertexIndexes* indexes, int vIdx){
	indexes->X = vIdx/BS;
	indexes->x = vIdx%BS;
}

/*
void printCompositeIndex(CompositeIndex* c){
    printf("I=%d i=%d J=%d j=%d\n", c->I, c->i, c->J, c->j);
}
*/


//Analiza si el camino P[idx] tiene efectivamente la distancia definida en D[idx]
//P ordenado por bloques, D ordenado por filas (estandar)
//Private
static int isEquivalentPath(int* P, int idx, TYPE* D, int n){
	int vAct, vNext, w, blockSize, dist, idxNoBlocking;
	VertexIndexes vNextIdxs;
	VertexIndexes vActIdxs;
	CompositeIndex cTarget;
	
	w = 0;
	blockSize = BS*BS;
	dist = 0;
	breakDownIndex(&cTarget, idx, n);
	idxNoBlocking = cTarget.I*BS*n + cTarget.J*BS + cTarget.i*n + cTarget.j;
	
	vAct = cTarget.I*BS+cTarget.i;
	getVertexIndexes(&vActIdxs, vAct);
	
	vNext = P[idx];
	getVertexIndexes(&vNextIdxs, vNext);
	
//	printf("vTarget = %d\n", cTarget.J*BS+cTarget.j);
	while( (w<n) && (vAct != cTarget.J*BS+cTarget.j) ){
	//	printf("vAct = %d, vNext = %d, distLoc = %d\n", vAct, vNext, D[vActIdxs.X*BS*n + vNextIdxs.X*BS + vActIdxs.x*n + vNextIdxs.x]);
		dist+= D[vActIdxs.X*BS*n + vNextIdxs.X*BS + vActIdxs.x*n + vNextIdxs.x];
		vAct = vNext;
		vActIdxs = vNextIdxs;
		vNext = P[vActIdxs.X*BS*n + cTarget.J*blockSize + vActIdxs.x*BS + cTarget.j];
		getVertexIndexes(&vNextIdxs, vNext);
		w++;
	}
//	printf("w = %d, dist = %d, D[idxNoBlocking] = %d.\n", w, dist, D[idxNoBlocking]);
	return ( (w<n) && (dist == D[idxNoBlocking]) );
}

//Comparacion custom. A ordenado por bloques, B ordenado por filas (estandar). Grafos de tipo int. "distRef" es el grafo de distancias minimas de referencia.
//Public
int intGraphsWBlocksEquals(int* A, int* B, int n, TYPE* distRef){
	int I,J,i,j,blockSize,r;
	r = n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<BS; i++)
				for(j=0; j<BS; j++)
					if(A[I*n*BS + J*blockSize + i*BS + j] != B[I*n*BS + J*BS + i*n + j])
						return isEquivalentPath(A, I*n*BS + J*blockSize + i*BS + j, distRef, n);

	return 1;
}

//Copia plana. Grafos de tipo TYPE.
//Public
void copyTYPEGraph(TYPE* A, TYPE* B, int n){
    INT64 i, vectorLength;
    vectorLength = (INT64)n*(INT64)n;
    for(i=0; i<vectorLength; i++)
        A[i] = B[i];
}

//Copia plana. Grafos de tipo int.
//Public
void copyIntGraph(int* A, int* B, int n){
    INT64 i, vectorLength;
    vectorLength = (INT64)n*(INT64)n;
    for(i=0; i<vectorLength; i++)
        A[i] = B[i];
}

//Copia el grafo B ordenado por filas (estandar) al grafo A ordenado por bloques. Grafos de tipo TYPE.
//Public
void TYPEGraphFromRowsToBlocks(TYPE* A, TYPE* B, int n){
	INT64 I,J,i,j,blockSize,r;
	r = n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<BS; i++)
				for(j=0; j<BS; j++)
					A[I*n*BS+J*blockSize+i*BS+j] = B[I*n*BS+J*BS+i*n+j];
}

//Copia el grafo B ordenado por filas (estandar) al grafo A ordenado por bloques. Grafos de tipo int.
//Public
void intGraphFromRowsToBlocks(int* A, int* B, int n){
	INT64 I,J,i,j,blockSize,r;
	r = n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<BS; i++)
				for(j=0; j<BS; j++)
					A[I*n*BS+J*blockSize+i*BS+j] = B[I*n*BS+J*BS+i*n+j];
}

//Copia el grafo B ordenado por bloques al grafo A ordenado por filas (estandar). Grafos de tipo TYPE.
//Public
void TYPEGraphFromBlocksToRows(TYPE* A, TYPE* B, int n){
	INT64 I,J,i,j,blockSize,r;
	r = n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<BS; i++)
				for(j=0; j<BS; j++)
					A[I*n*BS+J*BS+i*n+j] = B[I*n*BS+J*blockSize+i*BS+j];
}

//Copia el grafo B ordenado por bloques al grafo A ordenado por filas (estandar). Grafos de tipo int.
//Public
void intGraphFromBlocksToRows(int* A, int* B, int n){
	INT64 I,J,i,j,blockSize,r;
	r = n/BS;
	blockSize = BS*BS;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<BS; i++)
				for(j=0; j<BS; j++)
					A[I*n*BS+J*BS+i*n+j] = B[I*n*BS+J*blockSize+i*BS+j];
}
