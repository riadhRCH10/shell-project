#include"errorHandler.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


#include"./parser.h" 

// Function where the system command is executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork(); 
  
    if (pid == -1) {
        errorHandling(2, parsed[0]); 
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
           errorHandling(0, parsed[0]);
        }
        //exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL); 
        return;
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

void ececArgsMultiple(char arr[10][100],int *arrsize) {
    char inputString[1000], *parsedArgs[100];

    for (int i = 0; i <= *arrsize; i++)
    {
        printf("\nexecutor: command %d : %s",i,arr[i]);
        strcpy(inputString, arr[i]);
        parseSpace(inputString,parsedArgs);
        printf("\n");
        execArgs(parsedArgs);
    }
}