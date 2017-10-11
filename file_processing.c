#include "file_processing.h"
#include "environment.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* 
	history file section
*/
FILE *log_out;
FILE *out;
FILE *file;

/*
History file section
*/
void open_history_file()
{
    out = fopen(history, "a");
}

FILE* get_history_file()
{
    return out;
}

void close_history_file()
{
    fclose(out);
}


/* 
	log file section
*/
void open_log_file()
{
    log_out = fopen(loge, "a");
}

FILE* get_log_file()
{
    return log_out;
}

void close_log_file()
{
    fclose(log_out);
}


/* 
	CommandsBatch file section
*/
void open_commands_batch_file(char *fileName)
{
    char *ff;
    ff = (char *) malloc(512 * sizeof(char));
    memset(ff, 0, 512);
    /*strcat(ff, currDir);
    strcat(ff, "/");*/
    strcat(ff, fileName);
    file = fopen(ff ,"r");
}

FILE* get_commands_batch_file()
{
    return file;
}

void close_commands_batch_file()
{
    fclose(file);
}