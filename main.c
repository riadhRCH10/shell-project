
// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#include"libs/errorHandler.h"
#include"libs/executor.h"
  
#include"libs/parser.h" 

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define MAXCOMMANDS 10 // max commandes composé
  
// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")
  
// Greeting shell during startup
void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(5);
    clear();
}
  
// Function to take input
int takeInput(char* str)
{
    char* buf;
  
    buf = readline("");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}
  
// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    //char* username = getenv("USER");
    //char* desktop = getenv("DESKTOP_SESSION");
    //printf("\n");
    printf("\n%s %% ",cwd);
}
  
int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;

    char arr[MAXCOMMANDS][MAXLIST];
    int arrsize = 0;
    char delimiter[1];
    //= {"riadh","ls -l"}

    //init_shell();

    using_history();    /* initialize history */
  
    while (1) {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        // process
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped, arr, &arrsize, delimiter);
        // execflag returns zero if there is no command or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.
  
        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);
  
        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);

        if (execFlag == 3) {
            printf("\ncommands to execute: %d, delimiter is: %s\n", arrsize+1, delimiter);
            ececArgsMultiple(arr, &arrsize);
        }
        
            
    }
    return 0;
}