#include <defs.h>

//Comparacion plana. Grafos de tipo TYPE.
int TYPEGraphsEquals(TYPE* A, TYPE* B, int n);
//Comparacion plana. Grafos de tipo int.
int intGraphsEquals(int* A, int* B, int n);

//Comparacion custom. A ordenado por bloques, B ordenado por filas (estandar). Grafos de tipo TYPE.
int TYPEGraphsWBlocksEquals(TYPE* A, TYPE* B, int n);
//Comparacion custom. A ordenado por bloques, B ordenado por filas (estandar). Grafos de tipo int. "distRef" es el grafo de distancias minimas de referencia.
int intGraphsWBlocksEquals(int* A, int* B, int n, TYPE* distRef);

//Copia plana. Grafos de tipo TYPE.
void copyTYPEGraph(TYPE* A, TYPE* B, int n);
//Copia plana. Grafos de tipo int.
void copyIntGraph(int* A, int* B, int n);

//Copia el grafo B ordenado por filas (estandar) al grafo A ordenado por bloques. Grafos de tipo TYPE.
void TYPEGraphFromRowsToBlocks(TYPE* A, TYPE* B, int n);
//Copia el grafo B ordenado por filas (estandar) al grafo A ordenado por bloques. Grafos de tipo int.
void intGraphFromRowsToBlocks(int* A, int* B, int n);

//Copia el grafo B ordenado por bloques al grafo A ordenado por filas (estandar). Grafos de tipo TYPE.
void TYPEGraphFromBlocksToRows(TYPE* A, TYPE* B, int n);
//Copia el grafo B ordenado por bloques al grafo A ordenado por filas (estandar). Grafos de tipo int.
void intGraphFromBlocksToRows(int* A, int* B, int n);
