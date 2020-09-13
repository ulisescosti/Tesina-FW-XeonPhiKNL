#include <stdlib.h>

//Public
void* abs_malloc(INT64 vectorLength){
	return malloc(vectorLength);
}

//Public
void abs_free(void* vec){
	free(vec);
}
