#include <stdlib.h>

//Public
void* abs_malloc(INT64 vectorLength){
	#ifdef INTEL_ARC
		return _mm_malloc(vectorLength, MEM_ALIGN);
	#else
		return aligned_alloc(MEM_ALIGN, vectorLength);
	#endif
}

//Public
void abs_free(void* vec){
	#ifdef INTEL_ARC
		_mm_free(vec);
	#else
		free(vec);
	#endif
}
