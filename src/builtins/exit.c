#include <stdio.h>
#include <stdlib.h>

#include "../headers/builtins.h"

short exit2(char** args){
	int n = 0;
	if(args[1] != NULL) {
	  n = atoi(args[1]);
	  char firstChar = *(args[1]);
	  
	  if(n == 0 && (firstChar < '0' || firstChar > '9')) {
		fprintf(stderr, "exit: %s: numeric argument required\n",args[1]);
		return EXIT_FAILURE;
	  }
	  else if(args[2] != NULL) { 
		fprintf(stderr, "exit: too many arguments\n");
		return EXIT_FAILURE;
	  }
	}
	
	printf("exit\n");
	exit(n);

	return EXIT_FAILURE;
}
