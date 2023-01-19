extern void parseSpace(char* str, char** parsed);
extern int processString(char* str, char** parsed, char arr[10][100],int *arrsize, char delimiter[1]);
extern void openHelp();
extern int handleBuiltInCmd(char** parsed);
int parseMultiple(char* str, char arr[10][100],int *arrsize,char delimiter[1]);
