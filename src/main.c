#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <floyd_warshal.h>
#include <APSP_ADT.h>
#include <test_graphs.h>
#include <args_parser.h>

ProgramArgs pArgs;

//Para testing
void die(char* str){
	printf("%s\n",str);
	exit(0);
}

double getExecTime(double startTime, double endTime){
	return endTime - startTime;
}

double getGFlops(double startTime, double endTime, int n){
	double N = (double)n; //Porque si n es muy grande -> n*n*n se pasa de 2^31.. y ya que el resultado debe ser double, no tiene tanto sentido poner INT64
	return ((N*N*N*2)/(endTime - startTime))/1000000000;
}

void printTimeResults(double startTime, double endTime){
	double execTime, gFlops;
	execTime = getExecTime(startTime, endTime);
	gFlops = getGFlops(startTime, endTime, pArgs.n);
	
	if(pArgs.options.printCsvResult){
		printf("%.2f;%.4f\n", execTime, gFlops);
	} else {
		printf("Resultados de la prueba: %.1lf segs, %.3lf GFlops\n", execTime, gFlops);
	}
}

double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

char* getFailMsg(int status){
	switch(status){
		case 1: return "Grafo de distancias: OK. Grafo de rutas: Fail.";
		case 2: return "Grafo de distancias: Fail. Grafo de rutas: OK.";
		case 3: return "Grafo de distancias: Fail. Grafo de rutas: Fail.";
		default:{
			printf("El estado '%d' no es valido\n", status);
			exit(1);
		}
	}
}

//Compara ambos "registros resultado". Imprime si son iguales o no. En caso negativo ademas imprime en cuales grafos se diferencian.
void printResultStatus(t_APSP_graph* graphToCheck, t_APSP_graph* referenceGraph){
	int result = APSP_graph_equals(graphToCheck, referenceGraph);
	if(result == 0){
		printf("Resultado correcto!\n");
	} else {
		printf("********** Resultado incorrecto! **********\n");
		printf("%s\n", getFailMsg(result));
	}
}

void runTest(t_APSP_graph* G){
	double startTime, endTime;
	int printCsvRes = pArgs.options.printCsvResult;
	
	if(!printCsvRes){
		printf("Preparando grafos:\n");
		printf("    Cargando matriz de adyacencia desde disco . . . ");
	}
	readInputGraphFromFile(G->D, G->n, BS);
	if(!printCsvRes){
		printf("Hecho\n");
	}
	/*
	printTYPEGraphPartially(G->D, G->n, 32);
	printf("\n");
	exit(0); 
	*/
	#ifndef NO_PATH
		if(!printCsvRes){
			printf("    Preparando matriz P en memoria . . . ");
		}
		APSP_init_path_graph(G);
		if(!printCsvRes){
			printf("Hecho\n");
		}
	#endif
	if(!printCsvRes){
		printf("Ejecutando Floyd Warshal %s (%s): . . .\n", getFloydVersion(), getFloydName());
	}
	startTime = dwalltime();
	floydWarshall(G->D, G->P, G->n, pArgs.t);
	endTime = dwalltime();
	if(!printCsvRes){
		printf("Ejecucion finalizada. ");
	}
	printTimeResults(startTime, endTime);
}

void printGraphs(t_APSP_graph* G, t_APSP_graph* gReference){
	printf("G:\n");
	printTYPEGraphPartially(G->D, G->n, 32);
	printf("gReference:\n");
	printTYPEGraphPartially(gReference->D, gReference->n, 32);
}

void loadReferenceGraphs(t_APSP_graph* gReference){
	printf("Preparando matrices de referencia:\n");
	printf("    Cargando matriz D de referencia desde disco . . . ");
	readReferenceResultDistanceGraphFromFile(gReference->D, gReference->n);
	printf("Hecho\n");
	#ifndef NO_PATH
		printf("    Cargando matriz P de referencia desde disco . . . ");
		readReferenceResultPathGraphFromFile(gReference->P, gReference->n);
		printf("Hecho \n");
	#endif
}

void run(){
//	TYPE* inputGraph; //Grafo de entrada.
	t_APSP_graph G;
	t_APSP_graph gReference; //Donde se vuelcan los resultados del algoritmo de referencia (el "confiable"). Luego se utiliza para compararlo con el resultado del algoritmo "no confiable" (cuando está todavía inmaduro en etapa de desarrollo).
	
	APSP_graph_new(&G, pArgs.n, versionImplementsBlocking());
	runTest(&G);
	if(pArgs.options.checkResult){
		APSP_graph_new(&gReference, pArgs.n, 0);
		loadReferenceGraphs(&gReference);
		printResultStatus(&G, &gReference);
//		APSP_graph_destroy(&gReference);
	}
//	APSP_graph_destroy(&G);
}

int main(int argc, char* argv[]){
	setvbuf(stdout, NULL, _IONBF, 0); //Desactiva el buffering automatico de stdout (para que no se espere el \n antes de imprimir cada linea)
	loadDefaultOptions(&pArgs.options);
	parseProgramArgs(&pArgs, argc, argv);
	validateProgramArgs(&pArgs, versionImplementsBlocking(), versionIsParallel());
	if(!pArgs.options.printCsvResult){
		printProgramArgs(&pArgs, versionImplementsBlocking(), versionIsParallel());
		printExtraInfo(&pArgs, sizeof(TYPE));
	}
	run();
	return 0;
}
