#include "variables.h"
#include "environment.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int counter = 0;
char ** variables_array;
char ** values_array;
const char * envVars[]=
{
    "PATH"
    ,"HOME"
    ,"TERM"
    ,"PS1"
    ,"MAIL"
    ,"TEMP"
    ,"JAVA_HOME"
    ,"ORACLE_HOME"
    ,"TZ"
    ,"PWD"
    ,"HISTFILE"
    ,"HISTFILESIZE"
    ,"HOSTNAME"
    ,"LD_LIBRARY_PATH"
    ,"USER"
    ,"DISPLAY"
    ,"SHEL"
    ,"TERMCAP"
    ,"OSTYPE"
    ,"MACHTYPE"
    ,"EDITOR"
    ,"PAGER"
    ,"MANPATH"
};
int len = 23;
int isEnvVariable(char * variable);
void removeChar(char *str, char c);
void validPathes(char *path);

/*
Search the comming variable in the table
*/
char* lookup_variable(char* key)
{
    int i = 0;
    if(!strcmp(key, "PATH")) {
        return path;
    }else if (!strcmp(key, "HOME")) {
        return home;
    } else if(isEnvVariable(key))
            {
                return getenv(key);
            }
    for(i=0; i < counter; i++)
        {
            if (!strcmp(variables_array[i],key)) {
                return values_array[i];
            }
        }
    return NULL;
}

/*
Set the input variable to the variable table
*/
void set_variable( char* key , char* value )
{
    int i;
    if (!isEnvVariable(key)) {
        for(i=0; i < counter; i++)
        {
            if(!strcmp(variables_array[i],key))
            {
                values_array[i]=(char *)malloc(strlen(value)+1);
                strcpy(values_array[i],value);
                return;
            }
        }
        variables_array[counter]=(char *)malloc(strlen(key)+1);
        strcpy(variables_array[counter],key);
        values_array[counter]=(char *)malloc(strlen(value)+1);
        strcpy(values_array[counter],value);
        counter++;
    } else {
        if(!strcmp(key, "PATH")) {
            memset(path, 0, 512);
            strcpy(path, value);
            validPathes(path);
        }else if (!strcmp(key, "HOME")) {
            strcpy(home, value);
        } else {
            echo("You can't use This");
        }
    }
}

/*
Print all the variables in the Table of Variables
*/
void print_all_variables( void )
{
	// you should implement this function
    int i = 0;
    for (i = 0; i < counter; i++) {
            printf("%s   :    %s\n", variables_array[i], values_array[i]);
        }
}


/*
Check if the key variable is an environment variable
*/
int isEnvVariable(char * variable)
{
    //int length=sizeof(envVars) / sizeof(envVars[0]);
    int i;
    for(i=0; i<len; i++)
    {
        if(strcmp(envVars[i],variable)==0)
            return 1;
    }
    return 0;
}