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
		printf ("An error occurred while opening the input file\n");
		printf("Path: \"%s\"\n", fullname);
		printf("%s\n", strerror(errno));
		exit(1);
	}
	readNumbers = fread(inputGraph, sizeOfType, (INT64)n*(INT64)n, file);
	
	if(readNumbers != (INT64)n*(INT64)n){
		printf("Could not read the expected amount of numbers in the file (read=%ld expected=%ld)\n", readNumbers, (INT64)n*(INT64)n);
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
void printIntGraph(int* graph, int n){
	printIntGraphPartially(graph, n, n);
}

//Public
void printTYPEGraph(TYPE* graph, int n){
	printTYPEGraphPartially(graph, n, n);
}
