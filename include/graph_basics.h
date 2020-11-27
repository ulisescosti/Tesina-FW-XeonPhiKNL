#include <defs.h>

//Flat comparison. For graphs of type TYPE.
int TYPEGraphsEquals(TYPE* A, TYPE* B, int n);
//Flat comparison. For graphs of type int.
int intGraphsEquals(int* A, int* B, int n);

//Custom comparison. A ordered by blocks, B ordered by rows (standard). Graphs of type TYPE.
int TYPEGraphsWBlocksEquals(TYPE* A, TYPE* B, int n);
//Custom comparison. A ordered by blocks, B ordered by rows (standard). Graphs of type int. "distRef" is the reference graph of minimum distances.
int intGraphsWBlocksEquals(int* A, int* B, int n, TYPE* distRef);

//Flat copy. For graphs of type TYPE.
void copyTYPEGraph(TYPE* A, TYPE* B, int n);
//Flat copy. For graphs of type int.
void copyIntGraph(int* A, int* B, int n);

//It copies the graph B, ordered by rows (standard) to graph A, ordered by blocks. Graphs of type TYPE.
void TYPEGraphFromRowsToBlocks(TYPE* A, TYPE* B, int n);
//It copies the graph B, ordered by rows (standard) to graph A, ordered by blocks. Graphs of type int.
void intGraphFromRowsToBlocks(int* A, int* B, int n);

//It copies the graph B, ordered by blocks to graph A, ordered by rows (standard). Graphs of type TYPE.
void TYPEGraphFromBlocksToRows(TYPE* A, TYPE* B, int n);
//It copies the graph B, ordered by blocks to graph A, ordered by rows (standard). Graphs of type int.
void intGraphFromBlocksToRows(int* A, int* B, int n);
