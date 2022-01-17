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

//Variables externes

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

//Fonctions
	
	long  howLongIsTheProcess (long start_nsec, long stop_nsec){
		long  time=(stop_nsec-start_nsec)/NANOTOMILI;
		return time;
		}
		
//Main

int main (){

	write (STDOUT_FILENO, msgHello, strlen(msgHello)); //Premier message affiché
	write (STDOUT_FILENO, prompt, strlen(prompt));	//Premier prompt sans affichage des valeurs de Exit

	while (1){
		keyboard_In_Size=read (STDIN_FILENO, keyboard_In ,CMD_SIZE); // ON lit l'entrée dans la console
		keyboard_In[keyboard_In_Size-1]='\0';
		if (strncmp(keyboard_In,exitOrder,CMD_SIZE)==0 || keyboard_In_Size == 0){ // Test qui permet de déterminer si l'entrée vaut "exit" ou bine CTRL+D qui coresspond a une chaine de caractères de taille nulle.
			write (STDOUT_FILENO, msgExit, strlen(msgExit));			
			break;
			}
			
		else{

			 c_pid= fork(); // on crée un processus fils grace à la fonction fork()
			 
			if (c_pid == -1){ // Si PID =-1 alors il y' as eu un erreur (exemple, le père est mort avant de faire le fork)
				perror("fork");
				exit(EXIT_FAILURE);
				}
				
			if (c_pid==0){ //Processus fils: on le reconnait car son PID est des 0
				clock_gettime(clk_id, &start); // On mesure le temps pour lequel el processus fils s'est lancé, ce sera notre référence pour afficher nos temps
			int i=0;
			char* firstArgv=strtok(keyboard_In,s); // On utilise strok pour decouper la chaine de caracatères entrés dans la console en arguments connus par Linux
			
			while (firstArgv != NULL){
				argv[i]=firstArgv; // Cette boucle While nous permet de couper peu a peu la chaine tout en stockant nos arguments afin de les executer dans la suite de notre code
				firstArgv= strtok(NULL,s);
				i++;
				}
				execvp(argv[0],argv); //On a modififé notre programme en remplacant execlp par execvp car celui-ci nous permet d'executer les ordres stockés dans un tableau
				exit (EXIT_FAILURE);			
				}
			else{ // processus parent: On le reconnait car il un PID different de 0 et positif.
				wait(&status); // On attend la fin du processus fils qui mêne a sa mort
				clock_gettime(clk_id, &stop); // On prend ici le temps qui suit la fin du processus fils afin de faire le calcul de notre temps
				double howLong=howLongIsTheProcess(start.tv_nsec,stop.tv_nsec); // On calcul la durée du process
				if (WIFEXITED(status)){ // Ce test nous permet d'afficher le prochain prompt dans le cas où le fils est mort après avoir finis son déroulement.
					sprintf (sonReturn,"enseash [exit: %d | %lf ms]  %%",WEXITSTATUS(status), howLong);
					write (STDOUT_FILENO, sonReturn, strlen(sonReturn));
					}
					
				else if (WIFSIGNALED(status)){// Ce test nous permet d'affcher le prochain prompts dans le cas où le fils est mort via un signal Kill.
					sprintf(sonSignal, "enseash [sig : %d  | %lf %%", WTERMSIG(status),((howLong)));
					write (STDOUT_FILENO, sonSignal, strlen(sonSignal));
					}

				}
			}
		}

	exit(EXIT_SUCCESS);
	}


