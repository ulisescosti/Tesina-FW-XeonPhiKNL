#include <omp.h>

char* getFloydName();
char* getFloydVersion();

//Public
int versionImplementsBlocking(){
	return 1; //True
}

//Public
int versionIsParallel(){
	return 1; //True
}
