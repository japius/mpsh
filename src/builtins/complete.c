#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../headers/lib.h"

hach_table* regexp_table = NULL;

char* get_complete(char *key){
  return get_from_table(key, regexp_table);
}

short complete_aux(char* key, char* value) {
  regex_t regexCompiled;
  
  if (regcomp(&regexCompiled, value, REG_EXTENDED)) {
	fprintf(stderr, "regexp is not valid : %s \n", value);
	regfree(&regexCompiled);
	return EXIT_FAILURE;
  }

  regexp_table = add_to_table(key, value, regexp_table);
  
  return EXIT_SUCCESS;
}

short complete (char ** args){
  if (args[1] == NULL || args[2] == NULL || args[3] != NULL) {
	fprintf(stderr, "not valid list of argument \n");
	return EXIT_FAILURE;
  }
  
  return complete_aux(args[1], args[2]);
}

short remove_complete_aux(char *key){
  return remove_from_table(key, regexp_table);
}

short remove_complete(char** args){
  for (int i = 1; args[i] != NULL; i++) {
	remove_complete_aux(args[i]);
  }
  
  return EXIT_SUCCESS;
}
