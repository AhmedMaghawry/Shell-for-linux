#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/wait.h>

#include "commands.h"
#include "environment.h"
#include "command_parser.h"
#include "file_processing.h"
#include "variables.h"

typedef enum{ false = 0 , true = 1 } bool ;

void start_shell(bool read_from_file, char* fileName);
void shell_loop(bool input_from_file);
void executeCommand(int flag, int numParm, char* arr[]);
void exIt(char* arr[], int num);
void exprEval(char* ex);
void validateEcho(int numParm, char *arr[]);
void display_history();
void append_history(char *command);
void append_log();
void validPathes(char *path);
void removeRepeat(char* homee,char *arra);
int exisitCommand(char arr[]);
void removeChar(char *str, char c);
void intializeVariables();
void checkCtrlD();
FILE * getFileExist(char * fileName, char * currDir);
int isFileExist(char * fileName, char * currDir);
void returnSpaces(char * path);
void terminateAllChilds();
void emptyPrevPathes();

//Flag for detecting if the next command is running in background or foreground
int background = 0;
//Number of Pathes want to search in it on a command
int pathes = 0;
//Pathes we want to search in
char **pathesArray;
//Array of the Variables keys
char ** variables_array;
//Array of the Variables values
char ** values_array;
//The Command to execute
char * command;

int main(int argc, char *argv[])
{
    //Signal to detect child termination
    signal(SIGCHLD, append_log);
    //Signal to prevent the effect of ctrl+D
    signal(SIGINT, terminateAllChilds);
    //Intialize the Environment variables which will uesd
    setup_environment();
    //Intialize the global variables
    intializeVariables();
    //Get the array of pathes to search the command in
    validPathes(path);
    if( argc > 1 ){
        start_shell(true, argv[1]);
    }
    else{
        start_shell(false, argv[0]);
    }
    return 0;
}

void start_shell(bool read_from_file, char* fileName)
{
    // let shell starts from home
	cd(home);
	if(read_from_file){
		//Open the batch file
        int re = isFileExist(fileName, currDir);
        if(re){
            open_commands_batch_file(fileName);
            shell_loop(true);
        } else {
            perror("The file not found");
            exite();
        }
	}
	else{
		shell_loop(false);
	}
}

void shell_loop(bool input_from_file)
{
	bool from_file = input_from_file;
	while(true){
        background = 0;
		if(from_file){
			//read next instruction from file
            if(!fgets(command, 512, get_commands_batch_file())) {
                //End of File
                from_file = false;
                close_commands_batch_file();
                continue;
            }
		}
		else{
			//read next instruction from console
            printf("Shell> ");
            fgets(command, 512, stdin);
            //checkCtrlD(l);
            if ( feof(stdin) ) {
                // user pressed ctrl-D
                checkCtrlD();
            }
		}
        //Append the Command to the History
            append_history(command);
        //Parse theCommand
        parse_command(command);
        int co = getNumParams();
        char** par = getPrams();
        char* commandArray[co+1];
        for(int i = 0; i < co; i++) {
            commandArray[i] = par[i];
        }
        commandArray[co] = NULL;
        int flag = getFlag();
        //Execute the Command
        executeCommand(flag, co, commandArray);	
    }
}

/*
Execute the incomming command by detecting the flag given from parse
and detect the type of the command to execute
*/
void executeCommand(int flag, int numParm, char* arr[]){
    int j = 0;
    switch(flag) {
        case 0 :
            //Comment
            //Do nothing 
            break;
        case 1 :
            //CD
            cd(arr[1]);
            break;
        case 2 :
            //Echo
            validateEcho(numParm, arr);
            break;
        case 3 :
            //Exper
            exprEval(arr[0]);
            break;
        case 4 :
            //Valid Comm
            exIt(arr, numParm);
            break;
        case 5 :
            // Exit
            exite();
            break;
        case 6 :
            //Error
            perror("There is an error in the Command, The Command doesn't exsist");
            break;
        case 7 :
            display_history();
            break;
    }
}

/*
Execute the simple command
*/
void exIt(char* arr[], int num) {
    char fi[1024] = "";
    char *temp[num+1];
    for (int i = 0; i < num; i++) {
        char te[1024] = "";
        strcpy(te, arr[i]);
        if(!strcmp(arr[i], "&")) {
            background = 1;
        } else {
            temp[i] = strdup(te);
        }
    }
    temp[num] = NULL;
    strcpy(fi, arr[0]);
    int val;
    if(fi[0] == '/') {
        val = 0;
    } else {
        val = exisitCommand(fi);
    }
    if(val > -1) {
        siginfo_t childStat;
        pid_t pid;
        pid = getpid();     // parent pid
        pid = fork();
        if(!pid) {
            if(fi[0] == '/') {
                execv(fi, temp);
            } else {
                char finaly[1024] = "";
                strcat(finaly ,pathesArray[val]);
                strcat(finaly, "/");
                strcat(finaly, fi);
                execv(finaly, temp);
            }
            char finaly[1024] = "";
            strcat(finaly ,pathesArray[val]);
            strcat(finaly, "/");
            strcat(finaly, fi);
            execv(finaly, temp);
        }
        if(!background){
            waitid(P_PID, pid, &childStat, WEXITED);
        }
        background = 0;
    } else {
        perror("There is an error in the Command, The Command doesn't exsist");
    }
}

/*
set the Variable to the variable table
*/
void exprEval(char* ex) {
    char* res[2];
    char *arra = strtok(ex, "=");
    res[0] = arra;
    arra = strtok (NULL, "=");
    res[1] = arra;
    set_variable(res[0], res[1]);
}

/*
Validate Echo to print the the whole message
*/
void validateEcho(int numParm, char *arr[]) {
    char finalRes[1024];
    strcpy(finalRes, "");
    int coo = 0;
    for(int i = 1; i < numParm; i++) {
        char re[1024];
        strcpy(re, arr[i]);
        strcat(finalRes,re);
        strcat(finalRes, " ");
    }
    removeChar(finalRes, '\"');
    removeChar(finalRes, '\'');
    echo(finalRes);
}

// Display all the history
void display_history()
{
    char command [514];
    FILE *oute = fopen(history ,"r+");
    if (oute == NULL)
        perror("ERROR : cann't open history file");
    else
    {
        char buff[512];
        while (fgets(buff, 512, oute))
            printf("%s", buff);
        fclose(oute);
    }
}

// add command to the end of hisotry file
void append_history(char *command)
{
    open_history_file();
    FILE* file = get_history_file();
    if(file!=NULL)
    {
        fputs(command, file);
        close_history_file();
    }
    else
    {
        perror(" ERROR : cann't open history file");
    }
}

// add log to the end of log file
void append_log()
{
    open_log_file();
    FILE *log_out = get_log_file();
    time_t rawtime;
    time (&rawtime);
    char *array = (char*)malloc(512 * sizeof(char));
    struct tm  *timeinfo = localtime (&rawtime);
    strftime(array, 512, "%d.%m.%y_%H:%M:%S", timeinfo);
    if(log_out!=NULL)
    {
        fprintf(log_out, "%s at %s\n", "child process was terminated", array);
        fclose(log_out);
    }
    else
    {
        perror("ERROR : cann't open log file");
    }
}

/*
Function to split the Path variable to extract the 
pathes we want to search in
*/
void validPathes(char *path) {
    char* test;
    pathes = 0;
    test = (char *)malloc(512 * sizeof (char));
    memset(test, 0, 512);
    strcpy(test, path);
    char *arra = strtok(test, ":");
    while (arra != NULL) {
        pathesArray[pathes] = arra;
        arra = strtok (NULL, ":");
        pathes++;
    }
    for(int i = 0; i < pathes; i++) {
        removeRepeat(home, pathesArray[i]);
    }
}

/*
There is some Pathes which hold the home directory by defult
so we don't need to add the home path to it so remove tha added path
*/
void removeRepeat(char* homee,char *arra) {
    if(arra[1] == 'h') {
        int ccc = 0;
        char res[512];
        strcpy(res, "");
        char *ar = strtok(arra, "/");
        while (ar != NULL) {
            if(ccc > 1) {
                strcat(res, "/");
                strcat(res, ar);
            }
            ar = strtok (NULL, "/");
            ccc++;
        }
        strcpy(arra, res);
    }
}

/*
Search for the command in the pathes
*/
int exisitCommand(char arr[]) {
    int good = -1;
    char tt[1024] = "";
    for(int i = 0; i < pathes; i++) {
        strcpy(tt, pathesArray[i]);
        strcat(tt, "/");
        strcat(tt, arr);
        FILE* ff = fopen(tt ,"r");
        if(ff) {
            good = i;
            fclose(ff);
            break;
        } else {
            good = -1;
        }
    }
    return good;
}

/*
Intialize the Global variables
*/
void intializeVariables() {
    variables_array=(char **)malloc(512 * sizeof (char *));
    values_array=(char **)malloc(512 * sizeof (char *));
    pathesArray=(char **)malloc(512 * sizeof (char *));
    command=(char *)malloc(512 * sizeof (char));
}

/*
Detect if Ctrl+D pressed to terminate
*/
void checkCtrlD() {
    exite();
}

/*
Check if the file Exisit
*/
int isFileExist(char * fileName, char * currDir) {
    char *finalDir;
    finalDir = (char *) malloc(512 * sizeof(char));
    memset(finalDir, 0, 512);
    /*strcat(finalDir, currDir);
    strcat(finalDir, "/");*/
    strcat(finalDir, fileName);
    FILE * file = fopen(finalDir ,"r+");
    if (file) {
        return 1;
    } else {
        return 0;
    }
}


/*
Check the file if Exist and return it
*/
FILE * getFileExist(char * fileName, char * currDir) {
    char *finalDir;
    memset(finalDir, 0, 512);
    strcat(finalDir, currDir);
    strcat(finalDir, "/");
    strcat(finalDir, fileName);
    FILE * file = fopen(finalDir ,"r+");
    if (file) {
        return file;
    } else {
        return NULL;
    }
}

/*
it is a special character to indicate the there is a space here
*/
void returnSpaces(char * path) {
    int i = 0;
    while(path[i] != '\0') {
        if(path[i] == '*')
            path[i] = ' ';
        i++;
    }
}

/*
Function to terminate child when ctrl+d presed
*/
void terminateAllChilds() {
    shell_loop(false);
}

/*
Empty the path array to prevent any changes
*/
void emptyPrevPathes() {
    for(int i = 0; i < pathes; i++) {
        strcpy(pathesArray[i], "");
    }
    pathes = 0;
}