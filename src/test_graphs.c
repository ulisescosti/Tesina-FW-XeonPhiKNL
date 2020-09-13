#include <test_graphs.h>
#include <stdio.h>
#include <stdlib.h>
#include <graph_basics.h>
#include <errno.h>
#include <string.h>
#include <omp.h>

//Private
static void readGraphFromFile(void* inputGraph, int n, char* fullname, int sizeOfType){
	size_t readNumbers;
	
	FILE* file = fopen(fullname,"rb");
	if(file == NULL){
		printf ("Error al abrir el archivo de entrada\n");
		printf("Ruta: \"%s\"\n", fullname);
		printf("%s\n", strerror(errno));
		exit(1);
	}
	readNumbers = fread(inputGraph, sizeOfType, (INT64)n*(INT64)n, file);
	
	if(readNumbers != (INT64)n*(INT64)n){
		printf("No se leyo la cantidad esperada de numeros del archivo (leida=%ld esperada=%ld)\n", readNumbers, (INT64)n*(INT64)n);
		if(ferror(file))
			perror( "Error reading file" );
		exit(1);
	}
	fclose(file);
}

//Public
void readInputGraphFromFile(TYPE* inputGraph, int n, int bs){
	char fullname[255];
	sprintf(fullname,"%s/%s/N%d-BS%d", FILES_FOLDER, INPUT_FILES_FOLDER, n, bs);
	readGraphFromFile(inputGraph, n, fullname, sizeof(TYPE));
}

//Public
void readReferenceResultDistanceGraphFromFile(TYPE* refDistGraph, int n){
	char fullname[255];
	sprintf(fullname,"%s/%s/dist-N%d", FILES_FOLDER, REFERENCE_FILES_FOLDER, n);
	readGraphFromFile(refDistGraph, n, fullname, sizeof(TYPE));
}

//Public
void readReferenceResultPathGraphFromFile(int* refPathGraph, int n){
	char fullname[255];
	sprintf(fullname,"%s/%s/path-N%d", FILES_FOLDER, REFERENCE_FILES_FOLDER, n);
	readGraphFromFile(refPathGraph, n, fullname, sizeof(int));
}

/*
//Public
void initInputGraph(TYPE* inputGraph, int bs, int n, int gd){
	srand(1);
	if(bs == 0)
		initInputGraphStandard(inputGraph, n, gd);
	else
		initInputGraphBlocked(inputGraph, bs, n, gd);
}*/


//---------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------- IMPRESION MATRICES ---------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------

//Public
void printIntGraphPartially(int* graph, int n, int section){
	int i, j;
	
	for(i=0; i<section; i++){
		for(j=0; j<section; j++){
			if(graph[i*n+j] == INFINITE)
				printf("INF ");
			else
				printf("%2d ", graph[i*n+j]);
		}
		printf("\n");
	}
	printf("\n");
}

//Public
void printTYPEGraphPartially(TYPE* graph, int n, int section){
	int i, j;
	
	for(i=0; i<section; i++){
		for(j=0; j<section; j++){
			if(graph[i*n+j] == INFINITE)
				printf("INF ");
			else
				printf("%3.0f ", graph[i*n+j]);
		}
		printf("\n");
	}
	printf("\n");
}

//Public
/*
void printIntGraphPartially(int* graph, int n, int section, ){
	int* gAux;
	if(bs != 0){
		gAux = (int*)aligned_alloc(MEM_ALIGN, (INT64)n*(INT64)n*sizeof(int));
		intGraphFromBlocksToRows(gAux, graph, n, bs);
		printIntRowGraphPartially(gAux, n, section);
		free(gAux);
	} else {
		printIntRowGraphPartially(graph, n, section);
	}
}

//Public
void printTYPEGraphPartially(TYPE* graph, int n, int bs, int section){
	TYPE* gAux;
	if(bs != 0){
		gAux = (TYPE*)aligned_alloc(MEM_ALIGN, (INT64)n*(INT64)n*sizeof(TYPE));
		TYPEGraphFromBlocksToRows(gAux, graph, n, bs);
		printTYPERowGraphPartially(gAux, n, section);
		free(gAux);
	} else {
		printTYPERowGraphPartially(graph, n, section);
	}
}
*/
//Public
void printIntGraph(int* graph, int n){
	printIntGraphPartially(graph, n, n);
}

//Public
void printTYPEGraph(TYPE* graph, int n){
	printTYPEGraphPartially(graph, n, n);
}
