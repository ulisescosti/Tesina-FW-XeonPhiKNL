#include <floyd_warshal.h> //It implements this header file.
#include "../src/floyd_versions/common/opt_2-4.c"

//Public
char* getFloydName(){
	return "with vectorization using SSE";
}

//Public
char* getFloydVersion(){
	return "Opt-2";
}
