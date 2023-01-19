#include"errorHandler.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<readline/history.h>
#include"./parser.h" 
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define MAXCOMMANDS 10 // max commandes composé
// Function where the system command is executed
int execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork(); 
  
    if (pid == -1) {
        errorHandling(2, parsed[0]); 
        return 1;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
           errorHandling(0, parsed[0]);
           return 1;
        }
        //exit(0);
    } else { //comand is executed
        char command[100] = "";
        while (*parsed)
        {
            strcat(command, parsed[0]);
            strcat(command, " ");
            parsed++;
        }
        //add_history(command);
        updateStatus();
        // waiting for child to terminate
        wait(NULL); 
        return 0;
    }
}
// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2]; 
    pid_t p1, p2;
  
    if (pipe(pipefd) < 0) {
      errorHandling(3, parsed[0]);
        return;
    }
    p1 = fork();
    if (p1 < 0) {
    errorHandling(2, parsed[0]);
        return;
    }
  
    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
  
        if (execvp(parsed[0], parsed) < 0) {
          errorHandling(0, parsed[0]); //command1
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();
  
        if (p2 < 0) {
          errorHandling(2, parsed[0]);
            return;
        }
  
        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
             errorHandling(0, parsed[0]); //command2
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

int execArgsMultiple(char arr[10][100],int *arrsize, char delimiter[1]) {
    char inputString[1000], *parsedArgs[100];

    if (strcmp(delimiter, "||") == 0) 
    {
        int i = 0;
        int executed = 1;
        while (i <= *arrsize && executed == 1)
        {
            printf("\nexecutor: command %d : %s",i+1,arr[i]);
            strcpy(inputString, arr[i]);
            parseSpace(inputString,parsedArgs);
            printf("\n");
            if (ownCmdHandler(parsedArgs)) {
                executed = 0; 
            }else {
                executed = execArgs(parsedArgs);
            }  
            i++;
        }

    } else if (strcmp(delimiter, "&&") == 0) 
    {
        printf("\ncommands to execute: %d\n", *arrsize+1);
        int i = 0;
        int executed = 0;
        while (i <= *arrsize && executed == 0)
        {
            printf("\nexecutor: command %d : %s",i+1,arr[i]);
            strcpy(inputString, arr[i]);
            parseSpace(inputString,parsedArgs);
            printf("\n");
            if (ownCmdHandler(parsedArgs)) {
                executed = 0;
            } else {
                executed = execArgs(parsedArgs);
            }
            
            if (executed != 0 && i<*arrsize )  {
                printf("\ncommand %d failed, cannot continue..\n",i+1);
            }
            i++;
        }

    } else {
        printf("\ncommands to execute: %d\n", *arrsize+1);
        for (int i = 0; i <= *arrsize; i++)
        {
            printf("\nexecutor: command %d : %s",i+1,arr[i]);
            strcpy(inputString, arr[i]);
            parseSpace(inputString,parsedArgs);
            printf("\n");
            if (ownCmdHandler(parsedArgs)) {
               
            } else {
                execArgs(parsedArgs);
            }
        }
    }
    return 0;
    
}


void batchMode(char str[1000]) {
    char inputString[MAXCOM], *parsedArgs[1000];
    char* parsedArgsPiped[MAXLIST];
    int execFlag0 = 0;
    char *args; 
    char arr[MAXCOMMANDS][MAXLIST];
    int arrsize = 0;
    char delimiter[1];
    FILE *fp = fopen(str, "r");
    
    if (fp!=NULL){
        char fline[200];
        char **fargs;
        while(fgets(fline, sizeof(fline), fp) != NULL){
            strtok(fline,"\n\r");
            execFlag0 = processString(fline, parsedArgs, parsedArgsPiped, arr, &arrsize, delimiter);
            if (execFlag0 == 1) {
            //printf("%s",fline);
            execArgs(parsedArgs);
            }

            if (execFlag0 == 3) {
                execArgsMultiple(arr, &arrsize, delimiter);
            }         	
	    }
    if (ftell(fp) == 0) {
        printf("File is empty!");
    }
    fclose(fp); 
    }  
}