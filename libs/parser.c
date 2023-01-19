#include"parser.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#include"./errorHandler.h"

#define MAXLIST 100

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
        "\n>   cd"
        "\n>   ls"
        "\n>   any commands available in UNIX shell"
        "\n"
        "\n>Built-in commands:"
        "\n>   hello"
        "\n>   quit"
        "\n>   help"
        "\n>   history"
        );
  
    return;
}

int handleBuiltInCmd(char** parsed)
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

        HISTORY_STATE *myhist = history_get_history_state();

        HIST_ENTRY **mylist = history_list();

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

int processString(char* str, char** parsed, char arr[10][100],int *arrsize, char delimiter[1])
{
    char inputted[1000];
    strcpy(inputted, str);

    if (strstr(str, "||") != NULL) {
        parseMultiple(str, arr, arrsize, "||");
        strcpy(delimiter,"||");
        return 2;
    }

    if (strstr(str, "&&") != NULL) {
        parseMultiple(str, arr, arrsize, "&&");
        strcpy(delimiter,"&&");
        return 2;
    }

    if (strstr(str, ";") != NULL) {
        parseMultiple(str, arr, arrsize, ";");
        strcpy(delimiter,";");
        return 2;
    }

    if (access(str, F_OK) == 0) {
        return 3;
    }

    if (strstr(str, "./") || strstr(str, "/")) {
        errorHandling(4,str);
        return 0;
    } 

    if (strstr(str, ">") || strstr(str, "<") || strstr(str, "|")) {
        return 4;
    } 

    parseSpace(str, parsed);
    
  
    if (handleBuiltInCmd(parsed)) {
        add_history(inputted);
        return 0;
    } else {
        return 1;
    }
}