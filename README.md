#Shell And System calls 
<hr>
#Problem Overview
You are required to implement a command line interpreter (i.e., shell). The shell should display a user prompt, for example: Shell>, at which a user can enter for example, ls -l command, as follows:
Shell> ls -l. Next, your shell creates a child process to execute this command. Finally, when its execution is finished, it prompts for the next command from the user.
A Unix shell is a command-line interpreter that provides a traditional user interface for the Unix operating system and for Unix-like systems. The shell can run in two modes: interactive and batch.
In the shell interactive mode, you start the shell program, which displays a prompt (e.g. Shell>) and the user of the shell types commands at the prompt. Your shell program executes each of these commands and terminates when the user enters the exit command at the prompt. In the shell batch mode, you start the shell by calling your shell program and specifying a batch file to execute the commands included in it. This batch file contains the list of commands (on separate lines) that the user wants to execute. In batch mode, you should not display a prompt, however, you will need to echo each line you read from the batch file (print it) before executing it. This feature in your program is to help debugging and testing your code. Your shell terminates when the end of the batch file is reached or the user types Ctrl-D.
Commands submitted by the user may be executed in either the foreground or the background.
User appends an & character at the end of the command to indicate that your shell should execute it in the background. For example, if the user of your shell program enters Shell> myCommand, your shell program should execute ”myCommand” in the foreground, which means that your shell program waits until the execution of this command completes before it proceeds and displays the
next prompt. However, if the user of your shell program enters Shell> myCommand &, your shell program should execute ”myCommand” in background, which means that your shell program starts executing the command, and then immediately returns and displays the next prompt while the command is still running in the background.
<hr>
#How  the code organized
The code is divided to many sequential steps as follow :
- First setup the environment by initializing the environment variables to use it in the code and show its affect on the other command like PATH and HOME and also save the directory of the project to use it in saving the history and log files.
- Release the signals detectors to detect the signals which generated from the child when terminates and also detect if Ctrl+C pressed to disable its effect.
- Get the paths which will search the coming command in to insure it is valid command to use.
- Detect the mode of the execution (Batch or interactive).
-Fetch the command-line and append it in history file.
- Do some basic some operations which will be explained later (In main Functions Section) to validate the command and make it ready to be executed where while parsing will detect its type (Comment, cd, echo, Expression, exit, Valid Command, Invalid Command) by setting a flag of the type.
- Start executing the parsed command depending on the flag.
- The execution of the command will be in child process which will created after insure that the command is good to execute.
- There is two types of running the command ( Background , Foreground) depending on the end of the command if it ends with “&” it will be in background else will be in foreground.
- The Shell will terminate if it face “exit” command or Ctrl+d pressed.
The Code is organized in 6 main files :
1- main : which contains the main processes and run from it.
2- commad-parser : parse the command to make it good to execute.
3- commands : contains the execution of the cd command , echo command and exit command.
4- environment : contains the main environment variables like PATH and HOME and current directory.
5- file_processing : handle the batch file which it reads from it the command , history file which we put the command executed in it and log file which contains the time where the child terminated in .
6- variables : class which handle the session variables which save the incoming variable from user to use it later in other commands and contains it in variable table.
<hr>
##Main functions
#in Main :
void executeCommand(int flag, int numParm, char* arr[])
Function which take the flag of the command ( which detect the type of the command “Talked about it before”) , number of parameters of the command and the parameters of the command array.
- It decide which function to use to execute the command.
void exIt(char* arr[], int num)
Function which take the number of parameters of the command and the parameters of the command array.
- It detect if the command in background or in foreground.
- Create a child process using fork() system call .
- If the command in foreground then wait the child to terminate else don’t wait it and execute next.
void validPathes(char *path)
Function which take the environment path and divide it with “:” and put the result in a string array.
int exisitCommand(char arr[])
Function which takes the first parameter of the parameter array and search it in the paths array.
- Search the command in the paths array and if exsist return the index of it .
<hr>
#in variables :
char* lookup_variable(char* key)
Function which search the key in the variable table and return it if exists.
- Check first if the variable is environment variable and if it is PATH or HOME return the variable saved in the program and if other return the environment variable from system .
- If isn’t then search it in the variable table if exist return it else return null.
void set_variable( char* key , char* value )
Function which take the key and the value we want to save in the variable table.
- Check first if the variable is environment variable and if it is PATH or HOME set the variable in the program.
- If isn’t then search it in the variable table if exist set it else create new variable.
<hr>
#in command_parser :
void parse_command( char* command )
Function which makes 4 main steps remove the extra spaces, replace the variables in the command with its value , generate the arguments array to use it in execv and flag setter which detect the type of the command using the first argument.
- Remove the extra spaces or tabs except the spaces in “ ”.
- loop on the command and detect ‘$’ and replace it with the variable value.
- Split the command with spaces and put the result parameters array.
-Decide the type of the command from the first parameter in the array.
<hr>
#How to compile and run the code
1- open the directory of the project.
2- open the terminal in this directory.
3- Run “make” command which will compile the makefile.
4- to run the interactive mode run “./shell” command.
5- to run the interactive mode run “./shell (dirctory of batch file)”.
6- enter the command you want to run.
7- to exit the shell enter “exit” command or press Ctrl+d.
