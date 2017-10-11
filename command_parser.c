#include "command_parser.h"
#include "commands.h"
#include "variables.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>

void remove_spaces(char *input);
int divideSpace(char* comm);
int isExpersion(char* input, int parms);
void handleVariables(char * command);
void replaceStars(char * comman);

char* arrayRes[100];
int prams;
char* first;
/*
it is a flag to indicate which command is it as follow :
0 = Comment
1 = CD
2 = ECHO
3 = Expresion
4 = Valid Command
5 = Exit
6 = Error Command
*/
int flag = 6;
/*
Parse the command by removing the extra spaces and replace variables
and divide the command to get the params
and Detect the type of the path
*/
void parse_command( char* command )
{
    if(strcmp(command,"\n")) {
        //Remaove unneeded spaces from the command
        remove_spaces(command);
        //Replace Variables with its value
        handleVariables(command);
        // Divide the command and put the result in arrayRes
        prams = divideSpace(command);
        //The Command
        first = arrayRes[0];
        if(!strcmp(arrayRes[0], "cd")) {
            flag = 1;
        }else if (!strcmp(arrayRes[0], "echo")) {
            flag = 2;
        }else if (!strcmp(arrayRes[0], "exit")) {
            flag = 5;
        }else if (first[0] == '#') {
            flag = 0;
        }else if (isExpersion(command, prams)) {
            //Replace * with spaces
            replaceStars(command);
            flag = 3;
        } else if (!strcmp(first, "history")) {
            flag = 7;
        }else if (!strcmp(first, "export")) {
            strcpy(command, arrayRes[1]);
            prams = divideSpace(arrayRes[1]);
            flag = 3;
        }else {
            flag = 4;
        }
    } else {
        //the command is a comment
        flag = 0;
    }
}


/*
Remove the unneeded spaces
*/
void remove_spaces( char *input)
{
    int i, j = 0;
    int first = 1;
    int doubleCouts = 0;
    char *temp;
    temp = (char *)malloc(512 * sizeof (char));
    memset(temp, 0, 512);
    int k = 0;
    while(input[k] != '\0') {
        if(input[k] == '\t')
            input[k] = ' ';
        k++;
    }
    for (i = 0; input[i] != '\0'; i++) {
        if(input[i] == '\"') {
            doubleCouts = !doubleCouts;
        } else {
        if(input[i] == '\n') {
            //Skip it
        }else if (input[i] != ' ' || doubleCouts) {
            if(input[i] == ' ' && doubleCouts) {
                temp[j++] = '*';
            } else {
                temp[j++] = input[i];
                first = 1;
            }
        } else {
            if(first && i != 0 && input[i] != '\n') {
                temp[j++] = ' ';
            }
            first = 0;
        }
    }
    }
    temp[j] = '\0';
    strcpy(input, temp);
}

/*
Divide the command to parameters to deal with.
*/
int divideSpace(char* comm) {
    char *arra = strtok(comm, " ");
    int co = 0;
    while (arra != NULL) {
        arrayRes[co] = arra;
        arra = strtok (NULL, " ");
        co++;
    }
    return co;
}

/*
Check if the Command is an experesion
*/
int isExpersion(char* input, int parms) {
    if(parms != 1)
        return 0;
    else {
        int i = 0;
        for (i = 0; input[i] != '\0'; i++) {
            if(input[i] == '=')
                return 1;
        }
    }
    return 0;
}

int getNumParams() {
    return prams;
}

char** getPrams() {
    return arrayRes;
}

int getFlag() {
    return flag;
}

/*
Replace * with spaces
*/
void replaceStars(char * comman) {
    int j = 0;
    while(comman[j] != '\0') {
        if(comman[j] == '*')
            comman[j] = ' ';
        j++;
    }
}

/*
Handle the variables in the command
*/
void handleVariables(char * command) {
    char *fif;
    fif = (char *)malloc(512 * sizeof (char));
    memset(fif, 0, 512);
    int j = 0;
    int i = 0;
    while(command[i] != '\0') {
        if (command[i] == '$') {
            char *tempVariable;
            tempVariable = (char *)malloc(512 * sizeof (char));
            memset(tempVariable, 0, 512);
            int k = 0;
            while(isalnum(command[i]) || command[i] == '_' || command[i] == '$') {
                if(command[i] != '$') {
                    tempVariable[k] = command[i];
                    k++;
                }
                i++;
            }
            i--;
            char * valueVar = lookup_variable(tempVariable);
            if(valueVar != NULL)
                strcpy(tempVariable, valueVar);
            else 
                strcpy(tempVariable, "");
            int f = 0;
            while (tempVariable[f] != '\0') {
                fif[j] = tempVariable[f];
                j++;
                f++;
            }  
        } else {
            fif[j] = command[i];
            j++;
        }
        i++;
    }
    strcpy(command, fif);
}