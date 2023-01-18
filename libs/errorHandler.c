#include "errorHandler.h"
#include<stdio.h>
#include <string.h>

int attempts = 3;

void errorHandling(int errCode ,char command[]) {

    int NoOfOwnCmds = 5, i;
    char* ListOfOwnCmds[NoOfOwnCmds];
  
    ListOfOwnCmds[0] = "quit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "history";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (difference(command, ListOfOwnCmds[i]) == 1) { //print help !
            openHelpError();
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
        attempts--;
        if (attempts == 0) {
            printf("Read linux documents here:\nhttps://juliend.github.io/linux-cheatsheet/\n");
            attempts = 3;
        } else {
            printf("Could not execute the inputted command please try again\n");
        }
        break;
    }
    case 1: {
        printf("no sutch file or directory\n");
        break;
    }
    case 2: {
        printf("Failed forking child..\n");
        break;
    }
    case 3: {
        printf("Pipe could not be initialized\n");
        break;
    }
    default: {
        printf("could not resolve the error\n");
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

void openHelpError()
{
    puts("\n***HELP MENU***"
        "\nList of Commands supported:"
        "\n>   cd"
        "\n>   ls"
        "\n>   any commands available in UNIX shell"
        "\n"
        "\n>Built-in commands:"
        "\n>   hello"
        "\n>   quit"
        "\n>   help"
        "\n>   history"
        //"\n>    pipe handling"
        //"\n>    improper space handling"
        );
  
    return;
}

int updateStatus() {
    attempts = 3;
}