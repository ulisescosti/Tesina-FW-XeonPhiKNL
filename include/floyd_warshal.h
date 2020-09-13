#include <defs.h>

char* getFloydName();
char* getFloydVersion();
int versionImplementsBlocking(); //Returns boolean
int versionIsParallel(); //Returns boolean

//Funcion abstracta de reservado de espacio en memoria dinamica. Cada version del algoritmo la implementara de diferente manera.
void* abs_malloc(INT64 vectorLength);
//Idem, solo que ahora se trata de la funcion que libera memoria.
void abs_free(void* vec);

void floydWarshall(TYPE* D, int* P, int n, int t);
//void prepareEnvironment();
