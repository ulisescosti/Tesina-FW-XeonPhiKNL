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
static void throwArgsError(char* errorMsg){
	printf("There is an error in the arguments: %s\n", errorMsg);
	exit(1);
}

//Private
static void printArgsFormat(){
	printf("Missing argument\n");
	printf("1. Number of vertices\n");
	printf("2. Number of threads\n");
	printf(". [Options]\n");
}

//Private
static int parseN(const char* str){
	int n = atoi(str);
	if(n == 0)
		throwArgsError("The arg N must be an integer greater than zero");
	return atoi(str);
}

//Private
static int parseT(const char* str){
	int t = atoi(str);
	if(t <= 0)
		throwArgsError("The arg T must be an integer greater than zero");
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
			throwArgsError(strcat("Unknown received argument: ", str));
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
	if(pArgs->options.checkResult && pArgs->options.printCsvResult)
		throwArgsError("Options -checkRes and -printCsvRes are not allowed simultaneusly");
	if(pArgs->n < N_MIN)
		throwArgsError("Graph is too small. You must insert a greater value for N.");
	if(pArgs->n > N_MAX)
		throwArgsError("Graph is too big. You must choose a smaller value for N.");
	
	if(versionImplementsBlocking){
		r = pArgs->n/BS;
		if(pArgs->n%BS > 0)
			throwArgsError("The block size (BS) must be a divisor of the number of vertices (N)");
	/*	if(versionIsParallel){
			if((r%pArgs->t > 0) && (pArgs->t%r > 0))
				throwArgsError("The number of threads (T) must be a divisor or a multiple of the number of blocks per side (r)");
		}*/
	} else {
		if( versionIsParallel && (pArgs->n%pArgs->t > 0) )
			throwArgsError("The number of threads (T) must be a divisor of the number of vertices (N)");
	}
}

//Private
static void printSeparatingLine(){
	printf("=============================================================================\n");
}

//Public
void printProgramArgs(const ProgramArgs* pArgs, int versionImplementsBlocking, int versionIsParallel){
	printf("Excecuting Floyd-Warshall test with the following arguments:\n");
	printf("N = %d\n",pArgs->n);
	if(versionImplementsBlocking)
		printf("BS = %d\n",BS);
	if(versionIsParallel)
		printf("T = %d\n",pArgs->t);
	printf("GD = %d%%\n",GRAPH_DENSITY);
	printf("P matrix computed: ");
	#ifdef NO_PATH
		printf("No\n");
	#else
		printf("Yes\n");
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
	printf("Matrices of %dx%d. Each one (%d) occupies in memory: %.2f MB\n", n, n, numOfMatrices, matrixMemSize);
	printf("Total occupied memory space: >= %.2f MB\n", matrixMemSize*numOfMatrices);
	printSeparatingLine();
}
