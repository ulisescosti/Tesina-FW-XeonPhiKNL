#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <floyd_warshal.h>
#include <APSP_ADT.h>
#include <test_graphs.h>
#include <args_parser.h>

ProgramArgs pArgs;

//For testing
void die(char* str){
	printf("%s\n",str);
	exit(0);
}

double getExecTime(double startTime, double endTime){
	return endTime - startTime;
}

double getGFlops(double startTime, double endTime, int n){
	double N = (double)n; 
	//Double because if 'n' is too big -> n * n * n exceeds 2 ^ 31. Also, the result must be double, so it doesn't make much sense to use INT64 here.
	return ((N*N*N*2)/(endTime - startTime))/1000000000;
}

void printTimeResults(double startTime, double endTime){
	double execTime, gFlops;
	execTime = getExecTime(startTime, endTime);
	gFlops = getGFlops(startTime, endTime, pArgs.n);
	
	if(pArgs.options.printCsvResult){
		printf("%.2f;%.4f\n", execTime, gFlops);
	} else {
		printf("Test results: %.1lf secs, %.3lf GFLOPS\n", execTime, gFlops);
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
		case 1: return "Distances graph: OK. Paths graph: Fail.";
		case 2: return "Distances graph: Fail. Paths graph: OK.";
		case 3: return "Distances graph: Fail. Paths graph: Fail.";
		default:{
			printf("The state '%d' is not valid\n", status);
			exit(1);
		}
	}
}

//It compares both result registers and  prints if they are equal or not. If not, it also prints in which graphs they differ.
void printResultStatus(t_APSP_graph* graphToCheck, t_APSP_graph* referenceGraph){
	int result = APSP_graph_equals(graphToCheck, referenceGraph);
	if(result == 0){
		printf("Correct result!\n");
	} else {
		printf("********** Incorrect result! **********\n");
		printf("%s\n", getFailMsg(result));
	}
}

void runTest(t_APSP_graph* G){
	double startTime, endTime;
	int printCsvRes = pArgs.options.printCsvResult;
	
	if(!printCsvRes){
		printf("Preparing graphs:\n");
		printf("    Loading adjacency matrix from disk . . . ");
	}
	readInputGraphFromFile(G->D, G->n, BS);
	if(!printCsvRes){
		printf("Done\n");
	}
	/*
	printTYPEGraphPartially(G->D, G->n, 32);
	printf("\n");
	exit(0); 
	*/
	#ifndef NO_PATH
		if(!printCsvRes){
			printf("    Preparing P matrix in memory . . . ");
		}
		APSP_init_path_graph(G);
		if(!printCsvRes){
			printf("Done\n");
		}
	#endif
	if(!printCsvRes){
		printf("Executing Floyd Warshal %s (%s): . . .\n", getFloydVersion(), getFloydName());
	}
	startTime = dwalltime();
	floydWarshall(G->D, G->P, G->n, pArgs.t);
	endTime = dwalltime();
	if(!printCsvRes){
		printf("Execution completed. ");
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
	printf("Preparing reference matrices:\n");
	printf("    Loading D reference matrix from disk . . . ");
	readReferenceResultDistanceGraphFromFile(gReference->D, gReference->n);
	printf("Done\n");
	#ifndef NO_PATH
		printf("    Loading P reference matrix from disk . . . ");
		readReferenceResultPathGraphFromFile(gReference->P, gReference->n);
		printf("Done \n");
	#endif
}

void run(){
	t_APSP_graph G;
	t_APSP_graph gReference; //Here are stored the results of the reference algorighm (the "trustworthy"). Then it is used for making a comparison with the result of the algoritm "not trustworthy" (when it is imature yet at developement stage).
	
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
	setvbuf(stdout, NULL, _IONBF, 0); ///This disables the automatic buffering of stdout (so the program does not wait the \n before printing each line)
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
