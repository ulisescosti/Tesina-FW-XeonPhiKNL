#include <args_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <defs.h>

//Public
void loadDefaultOptions(Options* options){
	options->checkResult = 0; //False
	options->printCsvResult = 0; //False
}

//Private
static void throwParamError(char* errorMsg){
	printf("Error en los parámetros: %s\n", errorMsg);
	exit(1);
}

//Private
static void printArgsFormat(){
	printf("Missing parameter\n");
	printf("1. Number of vertices\n");
	printf("2. Number of threads\n");
	printf(". [Options]\n");
}

//Private
static int parseN(const char* str){
	int n = atoi(str);
	if(n == 0)
		throwParamError("El parametro N debe ser un numero entero mayor a cero");
	return atoi(str);
}

//Private
static int parseT(const char* str){
	int t = atoi(str);
	if(t <= 0)
		throwParamError("El parametro T debe ser un numero entero mayor a cero");
	return t;
}

//Private
static void parseOptions(Options* options, int argc, char* argv[], int numOfMandatoryArgs){
	int i;
	char* str;
	for(i=numOfMandatoryArgs; i<argc; i++){
		str = argv[i];
		if( (strcmp(str, "-checkRes") == 0) || (strcmp(str, "-CR") == 0) ){
			options->checkResult = 1; //True
		} else if( (strcmp(str, "-printCsvRes") == 0) || (strcmp(str, "-PC") == 0) ){
			options->printCsvResult = 1; //True
		} else {
			throwParamError(strcat("No se reconoce el parametro recibido: ", str));
		}
	}
}

//Public
void parseProgramArgs(ProgramArgs* pArgs, int argc, char* argv[]){
	int numOfMandatoryArgs = NUM_OF_MANDATORY_ARGS+1;
	if(argc < numOfMandatoryArgs){
		printArgsFormat();
		exit(1);
	}
	pArgs->n = parseN(argv[1]);
	pArgs->t = parseT(argv[2]);
	if(argc > numOfMandatoryArgs){
		parseOptions(&pArgs->options, argc, argv, numOfMandatoryArgs);
	}
}

//Public
void validateProgramArgs(const ProgramArgs* pArgs, int versionImplementsBlocking, int versionIsParallel){
	int r;
//	if(pArgs->options.checkResult && (pArgs->n > 8192))
//		throwParamError("Con -checkRes solo se admite un N maximo de 8192");
		//Con valores de N tan grandes tardaria demasiado en ser ejecutado por el algoritmo de referencia
	if(pArgs->options.checkResult && pArgs->options.printCsvResult)
		throwParamError("No se admiten las opciones -checkRes y -printCsvRes en simultaneo");
	if(pArgs->n < N_MIN)
		throwParamError("Grafo demasiado pequeño, debe elegir un N mas grande.");
	if(pArgs->n > N_MAX)
		throwParamError("Grafo demasiado grande, debe elegir un N mas chico.");
	
	if(versionImplementsBlocking){
		r = pArgs->n/BS;
		if(pArgs->n%BS > 0)
			throwParamError("El tamaño de bloque (BS) debe ser un divisor de la cantidad de vértices (N)");
		if(versionIsParallel){
		//	if(r%pArgs->t > pArgs->bs)
		//		throwParamError("La cantidad de threads (T) debe ser menor o igual que el tamaño de bloque (BS)");
				//Esto es debido a que en la fase 1 todos los threads ejecutan el bloque [0,0], por lo que si hay mas threads que filas en el bloque algunos de ellos quedarían ociosos.
			if((r%pArgs->t > 0) && (pArgs->t%r > 0))
				throwParamError("La cantidad de threads (T) debe ser un divisor o un multiplo de la cantidad de bloques por lado (r)");
		/*	if(pArgs->t > r)
				throwParamError("La cantidad de threads (T) debe ser menor o igual que la cantidad de bloques por lado (r)");*/
		}
	} else {
		if( versionIsParallel && (pArgs->n%pArgs->t > 0) )
			throwParamError("La cantidad de threads (T) debe ser un divisor de la cantidad de vértices (N)");
	}
}

//Private
static void printSeparatingLine(){
	printf("=============================================================================\n");
}

//Public
void printProgramArgs(const ProgramArgs* pArgs, int versionImplementsBlocking, int versionIsParallel){
	printf("Se ejecutará el test de Floyd-Warshal con los siguientes parametros:\n");
	printf("N = %d\n",pArgs->n);
	if(versionImplementsBlocking)
		printf("BS = %d\n",BS);
	if(versionIsParallel)
		printf("T = %d\n",pArgs->t);
	printf("GD = %d%%\n",GRAPH_DENSITY);
	printf("Se genera matriz de reconstruccion de caminos minimos: ");
	#ifdef NO_PATH
		printf("Si\n");
	#else
		printf("No\n");
	#endif
	printSeparatingLine();
}

//Public
void printExtraInfo(const ProgramArgs* pArgs, int sizeOfType){
	int n = pArgs->n;
	float matrixMemSize = ((float)n*n*sizeOfType)/(1024*1024);
	int numOfMatrices = 2;
	
	if(pArgs->options.checkResult)
		numOfMatrices = 4;
	printf("Matrices de %dx%d. Cada una (%d) ocupa en memoria: %.2f MB\n", n, n, numOfMatrices, matrixMemSize);
	printf("Espacio total ocupado en memoria: >= %.2f MB\n", matrixMemSize*numOfMatrices);
	printSeparatingLine();
}
