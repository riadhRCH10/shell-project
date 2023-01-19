#include"parser.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXLIST 100 // max number of commands to be supported

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;
  
    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
  
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
  
// function for finding pipe
  int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
  
    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

int parseMultiple(char* str, char arr[10][100],int *arrsize, char delimiter[1]) {

    int i = 0;
    while (strstr(str, delimiter) != NULL && i<10)
    {
        strcpy(arr[i], strsep(&str, delimiter));
        i++;
        if ( strcmp(delimiter,";") != 0 ) {
            str++;
        }
    }
    strcpy(arr[i], str);
    *arrsize = i;
    
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

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 5, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;
  
    ListOfOwnCmds[0] = "quit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "history";
  
    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg) {
    case 1:
        printf("\nGoodbye\n");
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nYou can try any linux commands here."
            "\nUse help to see the built in commands.\n",
            username);
        return 1;
    case 5:
        /* get the state of your history list (offset, length, size) */
        HISTORY_STATE *myhist = history_get_history_state();

        /* retrieve the history list */
        HIST_ENTRY **mylist = history_list();

        /* output history list */
        username = getenv("USER");
        printf ("\nsession history for %s\n\n", username);
        for (int i = 0; i < myhist->length; i++) { 
            printf ("%d  %s\n",i+1, mylist[i]->line);
        }
        putchar ('\n');
        return 1;

    default:
        break;
    }
  
    return 0;
}

int processString(char* str, char** parsed, char** parsedpipe,char arr[10][100],int *arrsize, char delimiter[1])
{
    char inputted[1000];
    strcpy(inputted, str);

    if (strstr(str, "||") != NULL) {
        parseMultiple(str, arr, arrsize, "||");
        strcpy(delimiter,"||");
        return 3;
    }

    if (strstr(str, "&&") != NULL) {
        parseMultiple(str, arr, arrsize, "&&");
        strcpy(delimiter,"&&");
        return 3;
    }

    if (strstr(str, ";") != NULL) {
        parseMultiple(str, arr, arrsize, ";");
        strcpy(delimiter,";");
        return 3;
    }

    if (access(str, F_OK) == 0) {
        return 4;
    }

    char* strpiped[2];
    int piped = 0;
  
    piped = parsePipe(str, strpiped);
  
    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
  
    } else {
  
        parseSpace(str, parsed);
    }
  
    if (ownCmdHandler(parsed)) {
        add_history(inputted);
        return 0;
    }
    else
        return 1 + piped;
}