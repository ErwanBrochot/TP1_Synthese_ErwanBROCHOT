#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CMD_SIZE 100

const char msgHello[]="Bienvenue dans le Shell ENSEA\nPour quitter, tapez 'exit'\n";
const char prompt[]="enseash % ";

int main (){
	write (STDOUT_FILENO, msgHello, sizeof(msgHello));
	write (STDOUT_FILENO, prompt, sizeof(prompt));


	exit(EXIT_SUCCESS);
	}
