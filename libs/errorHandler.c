#include "errorHandler.h"
#include<stdio.h>
#include <string.h>

void errorHandling(int errCode ,char command[]) {

    int NoOfOwnCmds = 5, i;
    char* ListOfOwnCmds[NoOfOwnCmds];
  
    ListOfOwnCmds[0] = "quit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "history";


    difference(command, ListOfOwnCmds[0]);

    //printf("\ndiff btw %s and %s is : %d, you need help!",command, ListOfOwnCmds[0], difference(command, ListOfOwnCmds[0]));

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (difference(command, ListOfOwnCmds[i]) <= 1) { //print help !
            openHelp();
            return;
        }
    }
    
    /*error codes:
    0: failed to execute command
    1: could not find a file
    2: forking error
    3: pipe error
    */
   switch(errCode) {
    case 0: {
        printf("\nerrCode: %d:", errCode);
        printf("\nCould not execute the inputted command please try again");
        break;
    }
    case 1: {
        printf("\nerrCode: %d", errCode);
        printf("\nno sutch file or directory");
        break;
    }
    case 2: {
        printf("\nerrCode: %d", errCode);
        printf("\nFailed forking child..");
        break;
    }
    case 3: {
        printf("\nerrCode: %d", errCode);
        printf("\nPipe could not be initialized");
        break;
    }
    default: {
        printf("\nerrCode: %d", errCode);
        printf("\ncould not resolve the error");
        break;
    }
   }
}

int difference(char first[], char second[]) {
    int diff = 0;
    int i=0;

    while (strlen(first) > 0 && strlen(second) > 0)
    {
        if (first[i] != second[i]) {
            diff += 1;
        }

        first ++;
        second++;
    }

    if (strlen(first) > 0) {
        diff += strlen(first);
    }

    if (strlen(second) > 0) {
        diff += strlen(second);
    }
    
    return diff;
}

void openHelp()
{
    puts("\n***HELP MENU***"
        "\nList of Commands supported:"
        "\n>cd"
        "\n>ls"
        "\n>all other general commands available in UNIX shell"
        "\n>hello"
        "\n>exit"
        "\n>pipe handling"
        "\n>improper space handling");
  
    return;
}