#include <defs.h>

char* getFloydName();
char* getFloydVersion();
int versionImplementsBlocking(); //Returns boolean
int versionIsParallel(); //Returns boolean

//Abstract malloc function (template pattern). Each program version implements it on its own.   
void* abs_malloc(INT64 vectorLength);
//Idem for free
void abs_free(void* vec);

void floydWarshall(TYPE* D, int* P, int n, int t);
