#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <defs.h>
#include <floyd_warshal.h>
#include <test_graphs.h>
#include <APSP_ADT.h>

#define N_MAX 65536
//#define N_MAX 16768

#define N_COUNT 3
const int n_array[N_COUNT] = {4096, 8192, 16384};
//const int n_array[N_COUNT] = {4096, 8192, 16384, 32768, 65536};

#define BS_COUNT 4
const int bs_array[BS_COUNT] = {0, 64, 128,256};


void initInputGraphStandard(TYPE* inputGraph, int n, int GD){
	INT64 i, j, edgePresence;
	TYPE valor;

	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			edgePresence = ((int)rand() % 100)+1;
			if(i>j){
				if(edgePresence <= GD){
					valor = rand() % 1000;
				} else {
					valor = INFINITE;
				}
				inputGraph[i*n+j] = valor;
				inputGraph[j*n+i] = valor;
			} else {
				if(j==i)
					inputGraph[i*n+j] = 0;
			}
		}
	} 
}

//It copies the graph B, ordered by rows (standard) to graph A, ordered by blocks. Graphs of type TYPE.
void graphFromRowsToBlocks(TYPE* A, TYPE* B, int n, int bs){
	INT64 I,J,i,j,blockSize,r;
	r = n/bs;
	blockSize = bs*bs;
	for(I=0; I<r; I++)
		for(J=0; J<r; J++)
			for(i=0; i<bs; i++)
				for(j=0; j<bs; j++)
					A[I*n*bs+J*blockSize+i*bs+j] = B[I*n*bs+J*bs+i*n+j];
}

void saveGraphToFile(void* graph, int n, char* fullname, int sizeOfType){
	size_t writtenNumbers;
	FILE* file = fopen(fullname,"wb");
	if(file == NULL){
		printf ("Error while reading the file: %s\n", strerror(errno));
		printf("Path: \"%s\"\n", fullname);
		exit(1);
	}
	writtenNumbers = fwrite(graph, sizeOfType, (INT64)n*(INT64)n, file);
	
	if(writtenNumbers != (INT64)n*(INT64)n){
		printf("The program could not write the expected amount of numbers in the file (written=%ld expected=%ld)\n", writtenNumbers, (INT64)n*(INT64)n);
		if(ferror(file))
			perror( "Error while writing the file" );
		exit(1);
	}
	fclose(file);
}

void saveInputGraphToFile(TYPE* graph, int n, int bs){
	char fullname[255];
	sprintf(fullname, "%s/%s/N%d-BS%d", FILES_FOLDER, INPUT_FILES_FOLDER, n, bs);
	printf("    Saving the input matrix (N=%d, BS=%d) to disk . . . ", n, bs);
	saveGraphToFile(graph, n, fullname, sizeof(TYPE));
	printf("Done\n");
}

void generateInputGraphs(){
	TYPE* inputGraph = (TYPE*)abs_malloc((INT64)N_MAX*(INT64)N_MAX*sizeof(TYPE));
	TYPE* blockedGraph = (TYPE*)abs_malloc((INT64)N_MAX*(INT64)N_MAX*sizeof(TYPE));
	TYPE* auxPtr;
	int i, j;
	

	for(i=0; i<N_COUNT; i++){
		printf("Creation of input graph (N=%d):\n", n_array[i]);
		printf("    Initializing input graph . . .");
		initInputGraphStandard(inputGraph, n_array[i], GRAPH_DENSITY);
		printf("Done\n");
		for(j=0; j<BS_COUNT; j++){
			printf("    Reordering matrix to blocks (BS=%d) . . .", bs_array[j]);
			if(bs_array[j] == 0){
				auxPtr = inputGraph;
			} else {
				graphFromRowsToBlocks(blockedGraph, inputGraph, n_array[i], bs_array[j]);
				auxPtr = blockedGraph;
			}
			printf("Done\n");
	/*		if(j==1){
				printTYPEGraphPartially(inputGraph, n_array[i], 32);
				printf("\n");
				exit(0); 
			} */
			saveInputGraphToFile(auxPtr, n_array[i], bs_array[j]);
		}
		printf("==================================================\n");
	}
//	free(auxGraph);
//	free(inputGraph);
}


//--------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------- REFERENCE GRAPH ----------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------

//Public
void saveReferenceResultDistanceGraphToFile(TYPE* refDistGraph, int n){
	char fullname[255];
	sprintf(fullname,"%s/%s/dist-N%d", FILES_FOLDER, REFERENCE_FILES_FOLDER, n);
	printf("    Saving the D reference matrix (N=%d) to disk . . . ", n);
	saveGraphToFile(refDistGraph, n, fullname, sizeof(TYPE));
	printf("Done\n");
}

//Public
void saveReferenceResultPathGraphToFile(int* refPathGraph, int n){
	char fullname[255];
	sprintf(fullname,"%s/%s/path-N%d", FILES_FOLDER, REFERENCE_FILES_FOLDER, n);
	printf("    Saving the P reference matrix (N=%d) to disk . . . ", n);
	saveGraphToFile(refPathGraph, n, fullname, sizeof(int));
	printf("Done\n");
}

void runReferenceFW(t_APSP_graph* gReference, int t){
	printf("Creation of reference result graphs (N=%d):\n", gReference->n);
	printf("    Loading adjacency matrix from disk . . . ");
	readInputGraphFromFile(gReference->D, gReference->n, 0);
	printf("Done\n");
	#ifndef NO_PATH
		printf("    Preparing P matrix in memory . . . ");
		APSP_init_path_graph(gReference);
		printf("Done\n");
	#endif
	printf("    Executing reference FW (N=%d): . . . ", gReference->n);
	floydWarshall(gReference->D, gReference->P, gReference->n, t);
	printf("Done\n");
}

void generateReferenceResultMatrices(int t){
	int i;
	t_APSP_graph gRef;
	APSP_graph_new(&gRef, N_MAX, 0);

	for(i=0; i<N_COUNT; i++){
		gRef.n = n_array[i];
		runReferenceFW(&gRef, t);
		saveReferenceResultDistanceGraphToFile(gRef.D, n_array[i]);
		#ifndef NO_PATH
			saveReferenceResultPathGraphToFile(gRef.P, n_array[i]);
		#endif
	}
	printf("Reference matrices have been generated successfully\n");
//	APSP_graph_destroy(&gRef);
}


//--------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------- MAIN -------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){
	char* str;
	//This disables the automatic buffering of stdout (so the program does not wait the \n before printing each line)
	setvbuf(stdout, NULL, _IONBF, 0);
	if(argc > 1){
		str = argv[1];
		if( (strcmp(str, "-checkRes") == 0) || (strcmp(str, "-CR") == 0) ){
			if(argc > 2){
				generateReferenceResultMatrices(atoi(argv[2]));
			} else {
				printf("The argument 'T' is missing\n");
			}
		} else {
			printf("Unable to recognise the received argument\n");
		}
	} else {
		generateInputGraphs();
	}
	return 0;
}
