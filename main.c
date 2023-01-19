#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libs/colors.h"
#include "libs/errorHandler.h"
#include "libs/executor.h"
#include "libs/init-shell.h"
#include "libs/parser.h"

#define MAXCOM 1000
#define MAXLIST 100
#define MAXCOMMANDS 10

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    int execFlag = 0;

    char arr[MAXCOMMANDS][MAXLIST];
    int arrsize = 0;
    char delimiter[1];

    char history_entry[MAXCOM];

    shell();

    using_history();

    while (1)
    {
        printDirectory();
        if (takeInput(inputString))
            continue;

        strcpy(history_entry, inputString);
        execFlag = processString(inputString, parsedArgs, arr, &arrsize, delimiter);

        if (execFlag == 1)
            if (executeSimple(parsedArgs) == 0)
            {
                add_history(history_entry);
            }

        if (execFlag == 2)
        {
            if (executeMultiple(arr, &arrsize, delimiter) == 0)
            {
                add_history(history_entry);
            }
        }

        if (execFlag == 3)
        {
            batchMode(inputString);
        }

        if (execFlag == 4)
        {
            printf(YELLOW "This version of shell does not support redirection or pipe." WHITE);
        }
    }
    return 0;
}