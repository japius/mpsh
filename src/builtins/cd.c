#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "../headers/builtins.h"

short cd_aux(char* new_location) {
  if (chdir(new_location) == 0)
	return EXIT_SUCCESS;
  
  perror("");
  return EXIT_FAILURE;
}

short cd(char** param) {
  if (param[1] != NULL && param[2] == NULL) {
	return cd_aux(param[1]);
  } else if (param[1] == NULL) {
	char* home = getenv("HOME");
	return cd_aux(home);
  }
  
  errno = E2BIG;
  perror("mpsh");
  
  return EXIT_FAILURE;
}
