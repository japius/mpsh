#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include "../headers/lib.h"


char* getPath() {
  char* path = malloc(PATH_MAX + 1);
  
  if (path!= NULL && getcwd(path, PATH_MAX) != NULL)
	return path;
  
  return NULL;
}

short pwd(char** args) {
  char* path = getPath();

  if (path != NULL) {
	printf("%s \n", path);
	free(path);
	return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
