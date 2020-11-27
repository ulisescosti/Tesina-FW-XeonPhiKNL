#include <defs.h>

#define FILES_FOLDER "input/files"
#define INPUT_FILES_FOLDER "input"
#define REFERENCE_FILES_FOLDER "ref"

//The param bs is needed because it could be 0 (blocking disabled) even when the BS constant is not 0 (for reference FW).
void readInputGraphFromFile(TYPE* inputGraph, int n, int bs);

void readReferenceResultDistanceGraphFromFile(TYPE* refDistGraph, int n);
void readReferenceResultPathGraphFromFile(int* refPathGraph, int n);

void printTYPEGraphPartially(TYPE* graph, int n, int section);
void printIntGraphPartially(int* graph, int n, int section);
void printTYPEGraph(TYPE* graph, int n);
void printIntGraph(int* graph, int n);
