#include <defs.h>

//"Registro resultado", sirve tanto para la salida como para la entrada de las funciones floyd. Luego de ejecutar un floyd queda en este registro el grafo/matriz de distancias minimas (D) y el grafo/matriz de reconstruccion de caminos minimos (P).
typedef struct{
	TYPE* D; //Grafo/matriz de distancias minimas.
	int* P; //Grafo/matriz de reconstruccion de caminos minimos.
	int n; //Cantidad de vertices (longitud de lado de las matrices D y P).
	int implementsBlocking;
} t_APSP_graph;

//Reserva en memoria espacio para el grafo de distancias y el de caminos.
void APSP_graph_new(t_APSP_graph* G, int n, int implementsBlocking);

//Libera la memoria.
void APSP_graph_destroy(t_APSP_graph* G);

//Inicializa el grafo de distancias y el de caminos: Copia inputGraph en G->D e inicializa G->P.
//G puede estar o no ordenado por bloques. inputGraph DEBE estar ordenado por filas (estandar).
//void APSP_graph_init(t_APSP_graph* G, TYPE* inputGraph);

//void APSP_import_distance_graph(t_APSP_graph* G, TYPE* inputGraph);
void APSP_init_path_graph(t_APSP_graph* G);


//Compara los grafos de 'A' con los de 'B'. Si son iguales entonces retorna 0, sino retorna 1, 2 o 3 segun cuales son los grafos que presentan diferencias (ver en el codigo).
//'A' puede estar ordenado por bloques, pero 'B' NO.
int APSP_graph_equals(t_APSP_graph* A, t_APSP_graph* B);
