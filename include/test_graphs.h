#include <defs.h>

#define FILES_FOLDER "input/files"
#define INPUT_FILES_FOLDER "input"
#define REFERENCE_FILES_FOLDER "ref"

void readInputGraphFromFile(TYPE* inputGraph, int n, int bs); //Se pasa por parametro bs porque puede venir 0 cuando la constante BS no es 0 (para FW de referencia).
void readReferenceResultDistanceGraphFromFile(TYPE* refDistGraph, int n);
void readReferenceResultPathGraphFromFile(int* refPathGraph, int n);

void printTYPEGraphPartially(TYPE* graph, int n, int section);
void printIntGraphPartially(int* graph, int n, int section);
void printTYPEGraph(TYPE* graph, int n);
void printIntGraph(int* graph, int n);

