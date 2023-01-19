#include "errorHandler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/history.h>
#include "./parser.h"
#include "colors.h"
#define MAXCOM 1000
#define MAXLIST 100
#define MAXCOMMANDS 10

int executeSimple(char **parsed)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        errorHandling(2, parsed[0]);
        return 1;
    }
    else if (pid == 0)
    {
        if (execvp(parsed[0], parsed) < 0)
        {
            errorHandling(0, parsed[0]);
            return 1;
        }
    }
    else
    {
        char command[100] = "";
        while (*parsed)
        {
            strcat(command, parsed[0]);
            strcat(command, " ");
            parsed++;
        }
        updateStatus();
        wait(NULL);
        return 0;
    }
}

int executeMultiple(char arr[10][100], int *arrsize, char delimiter[1])
{
    char inputString[1000], *parsedArgs[100];

    if (strcmp(delimiter, "||") == 0)
    {
        int i = 0;
        int executed = 1;
        while (i <= *arrsize && executed == 1)
        {
            printf("\nexecutor: command %d : %s", i + 1, arr[i]);
            strcpy(inputString, arr[i]);
            parseSpace(inputString, parsedArgs);
            printf("\n");
            if (handleBuiltInCmd(parsedArgs))
            {
                executed = 0;
            }
            else
            {
                executed = executeSimple(parsedArgs);
            }
            i++;
        }
    }
    else if (strcmp(delimiter, "&&") == 0)
    {
        printf(GREEN "\ncommands to execute:" WHITE);
        printf(GREEN " %d\n" WHITE, *arrsize + 1);
        int i = 0;
        int executed = 0;
        while (i <= *arrsize && executed == 0)
        {
            printf("\nexecutor: command %d : %s", i + 1, arr[i]);
            strcpy(inputString, arr[i]);
            parseSpace(inputString, parsedArgs);
            printf("\n");
            if (handleBuiltInCmd(parsedArgs))
            {
                executed = 0;
            }
            else
            {
                executed = executeSimple(parsedArgs);
            }

            if (executed != 0 && i < *arrsize)
            {
                printf(RED "\ncommand %d failed, cannot continue..\n" WHITE, i + 1);
            }
            i++;
        }
    }
    else
    {
        printf(GREEN "\ncommands to execute:" WHITE);
        printf(GREEN " %d\n" WHITE, *arrsize + 1);
        for (int i = 0; i <= *arrsize; i++)
        {
            printf("\nexecutor: command %d : %s", i + 1, arr[i]);
            strcpy(inputString, arr[i]);
            parseSpace(inputString, parsedArgs);
            printf("\n");
            if (handleBuiltInCmd(parsedArgs))
            {
            }
            else
            {
                executeSimple(parsedArgs);
            }
        }
    }
    return 0;
}

void batchMode(char str[1000])
{
    char inputString[MAXCOM], *parsedArgs[1000];
    int execFlag0 = 0;
    char *args;
    char arr[MAXCOMMANDS][MAXLIST];
    int arrsize = 0;
    char delimiter[1];
    FILE *fp = fopen(str, "r");

    if (fp != NULL)
    {
        char fline[200];
        char **fargs;
        while (fgets(fline, sizeof(fline), fp) != NULL)
        {
            strtok(fline, "\n\r");
            execFlag0 = processString(fline, parsedArgs, arr, &arrsize, delimiter);
            if (execFlag0 == 1)
                executeSimple(parsedArgs);

            if (execFlag0 == 2)
                executeMultiple(arr, &arrsize, delimiter);
        }
        if (ftell(fp) == 0)
        {
            errorHandling(3, str);
        }
        fclose(fp);
    }
}