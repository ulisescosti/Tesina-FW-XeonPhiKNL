#include <graph_basics.h>
#include <stdio.h>
#include <stdlib.h>

//Flat comparison. For graphs of type TYPE.
//Public
int TYPEGraphsEquals(TYPE* A, TYPE* B, int n){
	int vectorLength = n*n;
	for(int i=0; i<vectorLength; i++)
		if(A[i] != B[i])
			return 0;
	return 1;
}

//Flat comparison. For graphs of type int.
//Public
int intGraphsEquals(int* A, int* B, int n){
	int vectorLength = n*n;
	for(int i=0; i<vectorLength; i++)
		if(A[i] != B[i])
			return 0;
	return 1;
}

//Custom comparison. A ordered by blocks, B ordered by rows (standard). Graphs of type TYPE.
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

//Analyze if the path P[idx] actually has the minimum distance stored in D[idx]
//P ordered by blocks, D ordered by rows (standard)
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

//Custom comparison. A ordered by blocks, B ordered by rows (standard). Graphs of type int. "distRef" is the reference graph of minimum distances.
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

//Flat copy. For graphs of type TYPE.
//Public
void copyTYPEGraph(TYPE* A, TYPE* B, int n){
    INT64 i, vectorLength;
    vectorLength = (INT64)n*(INT64)n;
    for(i=0; i<vectorLength; i++)
        A[i] = B[i];
}

//Flat copy. For graphs of type int.
//Public
void copyIntGraph(int* A, int* B, int n){
    INT64 i, vectorLength;
    vectorLength = (INT64)n*(INT64)n;
    for(i=0; i<vectorLength; i++)
        A[i] = B[i];
}

//It copies the graph B, ordered by rows (standard) to graph A, ordered by blocks. Graphs of type TYPE.
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

//It copies the graph B, ordered by rows (standard) to graph A, ordered by blocks. Graphs of type int.
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

//It copies the graph B, ordered by blocks to graph A, ordered by rows (standard). Graphs of type TYPE.
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

//It copies the graph B, ordered by blocks to graph A, ordered by rows (standard). Graphs of type int.
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
