#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/lib.h"
#include "../headers/builtins.h"

void printAllAlias(){
  if (alias_table == NULL)
	return;
  
  for(int i = 0; i < alias_table->length; i++) {
	str_pair* pair = alias_table->tab[i];
	
	if (pair != NULL && !pair->is_free)
	  printf("%s='%s'\n",pair->key,pair->value);
  }
}

char* get_alias(char *key){
  return get_from_table(key, alias_table);
}

short is_valid_alias(char*key,char* value) {
  char* courant = value;

  while (courant != NULL) {
	if(strcmp(courant, key) == 0)
	  return 0;
	
	courant = get_alias(courant);
  }
  
  return 1;
}


short alias(char ** args){
  
  if(args[1] == NULL){
	printAllAlias();
	return EXIT_SUCCESS;
  }
	
  for(int i=1; args[i] != NULL; i++){
	
	char *arg = args[i];
	char *key=strtok(arg,"=");
	char *val=strtok(NULL,"\0");
	  
	if(key == NULL || val == NULL){
	  fprintf(stderr, "alias: %s : not found \n",arg);
	  return EXIT_FAILURE;			
	} else if (!is_valid_alias(key, val)){		  
	  fprintf(stderr, "alias: %s : not valid \n", arg);		  
	  return EXIT_FAILURE;
	}
		
	alias_table = add_to_table(key, val, alias_table);	 
  }
	
  return EXIT_SUCCESS;
}

short removeAlias(char *key){
  return remove_from_table(key, alias_table);
}

void clearAlias(){
  free_table(alias_table);
}

short unalias(char** args){
  
  if(args[1] == NULL) {
	clearAlias();
	return EXIT_SUCCESS;
  }
	
  for(int i=1; args[i] != NULL; i++){
	char *arg = args[i];
 
	if(!removeAlias(arg)){
	  fprintf(stderr, "unalias: %s : not found\n",arg);
	  return EXIT_FAILURE;
	}
  }
	
  return EXIT_SUCCESS;
}
