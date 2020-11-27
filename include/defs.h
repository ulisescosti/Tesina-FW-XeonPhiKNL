#include <stdint.h>
#include <limits.h>

#define TYPE float

#define INT64 int64_t

#define INFINITE SHRT_MAX
//If a smaller type than int is used (like short or char), then INFINITE value must be reduced, as this value should always be at most half of the maximum value of the type.

#define GRAPH_DENSITY 70
