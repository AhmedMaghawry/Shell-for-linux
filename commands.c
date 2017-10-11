#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "environment.h"
#include "commands.h"
#include "variables.h"

char finalPath[1024];
char currDi[1024];

int validPath(char * path);
void removeChar(char *str, char c);
void checkVarr(char res[]);
void returnSpaces(char * aa);
int isValid(char * pp);
void goodPath(char *fin);

/*
Command to change the directory when the path is valid
*/
void cd( char* path )
{
    //check if the user want to go home by cd only
    if(path == NULL || !strcmp(path, " ")){
        cd(home);
    }else if(validPath(path)){
        chdir(finalPath);
    }else {
        perror("Invalid Path");
    }
    /*char *const parmList[] = {"ls", "-l", NULL};
    execv("/bin/ls", parmList);*/
}  

/*
print on the console
*/
void echo( char* message )
{
	returnSpaces(message);
    printf("%s\n", message);
}

void exite () {
    exit(0);
}

/*
Check if the entered path is a good path to go to
*/
int validPath(char * path) {
    returnSpaces(path);
    strcpy(finalPath, path);
    int len;
    if(finalPath[0] == '~' && (finalPath[1] == '/' || finalPath[1] == NULL)) {
        strcpy(finalPath, "");
        strcat(finalPath, home);
        removeChar(path, '~');
        strcat(finalPath, path);
    } else if (finalPath[0] == '.' && finalPath[1] == '.') {
        getcwd(currDi, sizeof(currDi));
        strcpy(finalPath, "");
        strcat(finalPath, "/");
        char *arra = strtok(currDi, "/");
        int co = 0;
        char *temp[100];
        while (arra != NULL) {
            temp[co] = arra;
            arra = strtok (NULL, "/");
            co++;
        }
        for(int j = 0; j < co-1; j++) {
            strcat(finalPath, "/");
            strcat(finalPath, temp[j]);
        }
        removeChar(path, '.');
        strcat(finalPath, path);
    //the user entered ~username
    } else if (finalPath[0] == '~' && !(finalPath[1] == '/' || finalPath[1] == NULL)){
        if(isValid(finalPath)){
            goodPath(finalPath);
        }
    }
    struct dirent *pDirent;
    DIR *pDir;
    pDir = opendir(finalPath);
    if (pDir == NULL) {
        return 0;
    } else {
        closedir (pDir);
        return 1;
    }
}

/*
Remove a specific character from a string
*/
void removeChar(char *str, char c) {
    int i = 0;
    int j = 0;

    while (str[i]) {
        if (str[i] != c) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j]=0;
}

/*
Check if the string after ~ is the username
*/
int isValid(char * pp) {
        char* test;
        test = (char *)malloc(512 * sizeof (char));
        memset(test, 0, 512);
        int f = 0;
        char* homeTemp;
        homeTemp = (char *)malloc(512 * sizeof (char));
        memset(homeTemp, 0, 512);
        strcpy(homeTemp, home);
        char *arra = strtok(homeTemp, "/");
        while (arra != NULL) {
            test = arra;
            if (f == 1)
                break;
            arra = strtok (NULL, "/");
            f++;
        }
        char* arrtest;
        arrtest = (char *)malloc(512 * sizeof (char));
        memset(arrtest, 0, 512);
        strcpy(arrtest, pp);
        char *arra2 = strtok(arrtest, "/");
        removeChar(arra2,'~');
        if(!strcmp(test, arra2)) {
            return 1;
        }
        return 0;
}


/*
Make the path with ~Username good to execute
*/
void goodPath(char *fin) {
    char* test;
    test = (char *)malloc(512 * sizeof (char));
    memset(test, 0, 512);
    char* finall;
    finall = (char *)malloc(512 * sizeof (char));
    memset(finall, 0, 512);
    int f = 0;
    char *arra = strtok(fin, "/");
    while (arra != NULL) {
        strcpy(test, arra);
        if (f == 0) {
            strcpy(test, home);
            strcat(finall, test);
        } else {
            strcat(finall, "/");
            strcat(finall, test);
        }
        arra = strtok (NULL, "/");
        f++;
    }
    strcpy(fin, finall);
}