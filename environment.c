#include "environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *path;
char *home;
char history[1024];
char loge[1024];
char currDir[1024];

/*
Setup the important environment variables 
only handle path and home
put also the current directory
*/
void setup_environment( void )
{
	// you should implement this function
    path = getenv("PATH");
    home = getenv("HOME");
    getcwd(currDir, sizeof(currDir));
    strcpy(history, "");
    strcpy(history, currDir);
    strcat(history, "/history");
    strcpy(loge, "");
    strcpy(loge, currDir);
    strcat(loge, "/log");
    //printf("PATH :%s\n",(path!=NULL)? path : "getenv returned NULL");
    //printf("HOME :%s\n",(home!=NULL)? home : "getenv returned NULL");
    //printf("Curr :%s\n",(currDir!=NULL)? currDir : "getenv returned NULL");
}