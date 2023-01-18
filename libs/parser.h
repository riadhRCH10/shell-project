extern void parseSpace(char* str, char** parsed);
extern int processString(char* str, char** parsed, char** parsedpipe,char arr[10][100],int *arrsize, char delimiter[1]);
extern int parsePipe(char* str, char** strpiped);
extern void openHelp();
extern int ownCmdHandler(char** parsed);
int parseMultiple(char* str, char arr[10][100],int *arrsize,char delimiter[1]);
