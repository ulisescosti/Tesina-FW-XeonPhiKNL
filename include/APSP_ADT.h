#include <defs.h>

typedef struct{
	TYPE* D; //Matrix of minimum distances
	int* P; //Matrix of minimum paths
	int n; //Number of vertices (side length of matrices D and P).
	int implementsBlocking;
} t_APSP_graph;

//Allocs memory for D and P graphs
void APSP_graph_new(t_APSP_graph* G, int n, int implementsBlocking);

//Memory free
void APSP_graph_destroy(t_APSP_graph* G);

//It initialize the P graph
void APSP_init_path_graph(t_APSP_graph* G);

//It compares the graphs of 'A' and 'B'. If they are equal, it returns 0, else, it returns 1, 2 or 3, depending on which graphs have diferences (for further details, see the source code).
//'A' may be ordered by blocks, but not 'B'.
int APSP_graph_equals(t_APSP_graph* A, t_APSP_graph* B);
