#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include "../headers/builtins.h"


char* which(const char* filename) {
  char* liste_des_chemins = strdup(getenv("CHEMIN"));
  char* chemin = malloc(PATH_MAX + 1);

  char* token = strtok(liste_des_chemins, ":");

  while (token != NULL) {
	strcpy(chemin, token);
	strcat(chemin, "/");
	strcat(chemin, filename);

	if (access(chemin, F_OK) != -1)
	  return chemin;

	token = strtok(NULL, ":");	
  }

  return NULL;
}

short find_path(const char* filename) {
  char* chemin = which(filename);

  if (chemin == NULL)
	return 0;

  printf("%s is %s\n",filename ,chemin);
  free(chemin);
  
  return 1;
}

short type_aux(char* filename) {
  char* tab[] = {"exit","cd","pwd","alias","unalias",
				 "export","umask","history","complete","rmcomplete","type",NULL};
  
  char* alias = get_alias(filename);
  
  if (alias != NULL) {
    printf("%s is aliased to '%s' \n",filename,alias);
    return EXIT_SUCCESS;
  }
  
  
  for (int i = 0; tab[i] != NULL; i++) {
    if (strcmp(tab[i], filename) == 0) {
      printf("%s is a shell builtin \n",filename);
      return EXIT_SUCCESS;
    }
  }

  if (find_path(filename))
	return EXIT_SUCCESS;

  fprintf(stderr,"%s: not found \n",filename);
  return EXIT_FAILURE;
}

short type(char** args) {
  short ret_val = EXIT_SUCCESS;
  
  for (int i=1; args[i] != NULL; i++) {
    if (type_aux(args[i]) == EXIT_FAILURE)
      ret_val = 1;
  }

  return ret_val;
}
