#define NUM_OF_MANDATORY_ARGS 2
#define N_MIN 16
#define N_MAX 65536

typedef struct{
	int checkResult; //Boolean
	int printCsvResult; //Boolean
}Options;

typedef struct{
	int n;
	int t;
	Options options;
}ProgramArgs;

void loadDefaultOptions(Options* options);
void parseProgramArgs(ProgramArgs* pArgs, int argc, char* argv[]);
void validateProgramArgs(const ProgramArgs* pArgs, int versionImplementsBlocking, int versionIsParallel);
void printProgramArgs(const ProgramArgs* pArgs, int versionIsParallel, int versionImplementsBlocking);
void printExtraInfo(const ProgramArgs* pArgs, int sizeOfType);
