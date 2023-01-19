#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include "./colors.h"
#define clear() printf("\033[H\033[J")

void shell()
{
    clear();
    printf(CYAN "\n\n\n\t***************WELCOME TO OUR SHELL******************");
    printf("\n\n\tMade by: Hatem Dridi, Maram Djebbi and Riadh Chercheri");
    printf("\n\n\n\t******************************************************");
    printf("\n" WHITE);
    sleep(1);
}

int takeInput(char *str)
{
    char *buf;

    buf = readline("");
    if (strlen(buf) != 0)
    {
        strcpy(str, buf);
        return 0;
    }
    else
    {
        return 1;
    }
}

void printDirectory()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n%s %% ", cwd);
}