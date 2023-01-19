#include "errorHandler.h"
#include <stdio.h>
#include <string.h>
#include "./colors.h"
int attempts = 3;

void errorHandling(int errCode, char command[])
{

    int NoOfOwnCmds = 5, i;
    char *ListOfOwnCmds[NoOfOwnCmds];

    ListOfOwnCmds[0] = "quit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "history";

    for (i = 0; i < NoOfOwnCmds; i++)
    {
        if (difference(command, ListOfOwnCmds[i]) == 1)
        {
            openHelpError();
            return;
        }
    }

    switch (errCode)
    {
    case 0:
    {
        attempts--;
        if (attempts == 0)
        {
            printf(RED "Read linux documents here:\nhttps://juliend.github.io/linux-cheatsheet/\n" WHITE);
            attempts = 3;
        }
        else
        {
            printf(RED "Could not execute the inputted command please try again\n" WHITE);
        }
        break;
    }
    case 1:
    {
        printf(RED "No sutch file or directory\n" WHITE);
        break;
    }
    case 2:
    {
        printf(RED "Failed forking child..\n" WHITE);
        break;
    }
    case 3:
    {
        printf(RED "File is empty!\n" WHITE);
        break;
    }
    case 4:
    {
        printf(RED "File does not exists\n" WHITE);
        break;
    }
    default:
    {
        printf(RED "Could not resolve the error\n" WHITE);
        break;
    }
    }
}

int difference(char first[], char second[])
{
    int diff = 0;
    int i = 0;

    while (strlen(first) > 0 && strlen(second) > 0)
    {
        if (first[i] != second[i])
        {
            diff += 1;
        }

        first++;
        second++;
    }

    if (strlen(first) > 0)
    {
        diff += strlen(first);
    }

    if (strlen(second) > 0)
    {
        diff += strlen(second);
    }

    return diff;
}

void openHelpError()
{
    puts(YELLOW "\n***HELP MENU***"
                "\nList of Commands supported:"
                "\n>   cd"
                "\n>   ls"
                "\n>   any commands available in UNIX shell"
                "\n"
                "\n>Built-in commands:"
                "\n>   hello"
                "\n>   quit"
                "\n>   help"
                "\n>   history" WHITE);

    return;
}

int updateStatus()
{
    attempts = 3;
}