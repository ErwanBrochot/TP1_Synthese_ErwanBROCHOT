#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define CMD_SIZE 100


const char msgHello[]="Bienvenue dans le Shell ENSEA\nPour quitter, tapez 'exit'\n";
const char prompt[]="enseash % ";
const char exitOrder[]="exit";
const char msgExit[]="bye bye...\n";

int main (){
	char keyboard_In[CMD_SIZE];
	int status;
	char sonReturn[CMD_SIZE];
	char sonSignal [CMD_SIZE];
	
	write (STDOUT_FILENO, msgHello, strlen(msgHello));
	write (STDOUT_FILENO, prompt, strlen(prompt));

	while (1){
//		write (STDOUT_FILENO, prompt, sizeof(prompt));
		int keyboard_In_Size=read (STDIN_FILENO, keyboard_In ,CMD_SIZE);
		keyboard_In[keyboard_In_Size-1]='\0';
		if (strncmp(keyboard_In,exitOrder,CMD_SIZE)==0 || keyboard_In_Size == 0){
			write (STDOUT_FILENO, msgExit, strlen(msgExit));
			break;
			}
		else{
			pid_t c_pid= fork();
			if (c_pid == -1){
				perror("fork");
				exit(EXIT_FAILURE);
				}
			if (c_pid==0){ //Processus fils
				execlp(keyboard_In,keyboard_In,(char *) NULL);
				exit (EXIT_FAILURE);			
				}
			else{ // processus parent
				wait(&status);
				if (WIFEXITED(status)){
					sprintf (sonReturn,"enseash [exit: %d]  %%",WEXITSTATUS(status));
					write (STDOUT_FILENO, sonReturn, strlen(sonReturn));
					}
					
				else if (WIFSIGNALED(status)){
					sprintf(sonSignal, "enseash [sig : %d %%", WTERMSIG(status));
					printf("signal exit: %d\n", WTERMSIG(status));
					write (STDOUT_FILENO, sonSignal, strlen(sonSignal));
					}

				}
			}
		}

	exit(EXIT_SUCCESS);
	}

