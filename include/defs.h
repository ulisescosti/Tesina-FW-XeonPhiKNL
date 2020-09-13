#include <stdint.h>
#include <limits.h>

#define TYPE float
#define INT64 int64_t

#define INFINITE SHRT_MAX
//Si en TYPE se utiliza un tipo menor a int, como short o char, se debe entonces achicar el valor de INFINITE, ya que este debe ser a lo sumo la mitad del valor maximo de TYPE


#define GRAPH_DENSITY 70
