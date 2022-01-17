#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define CMD_SIZE 100
#define NANOTOMILI 1000000
#define SECONDTOMILI 1000


const char msgHello[]="Bienvenue dans le Shell ENSEA\nPour quitter, tapez 'exit'\n";
const char prompt[]="enseash % ";
const char exitOrder[]="exit";
const char msgExit[]="bye bye...\n";
const char s []= " ";
clockid_t clk_id= CLOCK_REALTIME;
char keyboard_In[CMD_SIZE];
int status, keyboard_In_Size;
char sonReturn[CMD_SIZE];
char sonSignal [CMD_SIZE];
struct timespec start, stop;
char* argv[CMD_SIZE];
char* firstArgv[CMD_SIZE];
pid_t c_pid;


	
	long double howLongIsTheProcess (time_t start_sec, time_t stop_sec, long start_nsec, long stop_nsec){
		long double time=((stop_sec-start_sec)*SECONDTOMILI)+((stop_nsec-start_nsec)/NANOTOMILI);
		return time;
		}

int main (){

	write (STDOUT_FILENO, msgHello, strlen(msgHello)); //Premier message affiché
	write (STDOUT_FILENO, prompt, strlen(prompt));	//Premier prompt sans affichage des valeurs de Exit

	while (1){
		keyboard_In_Size=read (STDIN_FILENO, keyboard_In ,CMD_SIZE);
		keyboard_In[keyboard_In_Size-1]='\0';
		if (strncmp(keyboard_In,exitOrder,CMD_SIZE)==0 || keyboard_In_Size == 0){
			write (STDOUT_FILENO, msgExit, strlen(msgExit));			
			break;
			}
			
		else{

			 c_pid= fork();
			 
			if (c_pid == -1){
				perror("fork");
				exit(EXIT_FAILURE);
				}
				
			if (c_pid==0){ //Processus fils
				clock_gettime(clk_id, &start);
			int i=0;
			char* firstArgv=strtok(keyboard_In,s);
			
			while (firstArgv != NULL){
				argv[i]=firstArgv;
				firstArgv= strtok(NULL,s);
				i++;
				}
				execvp(argv[0],argv);
				exit (EXIT_FAILURE);			
				}
			else{ // processus parent
				wait(&status);
				clock_gettime(clk_id, &stop);
				double howLong=howLongIsTheProcess(start.tv_sec,stop.tv_sec,start.tv_nsec,stop.tv_nsec);
				howLong=(stop.tv_nsec-start.tv_nsec)/NANOTOMILI;
				if (WIFEXITED(status)){
					sprintf (sonReturn,"enseash [exit: %d | %f ms]  %%",WEXITSTATUS(status), howLong);
					write (STDOUT_FILENO, sonReturn, strlen(sonReturn));
					}
					
				else if (WIFSIGNALED(status)){
					sprintf(sonSignal, "enseash [sig : %d  | %f %%", WTERMSIG(status),((howLong)));
					write (STDOUT_FILENO, sonSignal, strlen(sonSignal));
					}

				}
			}
		}

	exit(EXIT_SUCCESS);
	}


