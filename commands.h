#ifndef COMMANDS_H_   /* Include guard */
#define COMMANDS_H_

/* 
	- This function should be responsible for implementing the "cd" shell command
*/
void cd( char* path );  

/* 
	- This function should be responsible for implementing the "echo" shell command
	- Typically should lookup any variable included in the message
*/
void echo( char* message );

void exite();

#endif // COMMANDS_H_