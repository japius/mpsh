#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/lib.h"

cmd create_command(char *name, char **args){
	cmd c = malloc(sizeof(command));
	if(c == NULL)
	  return NULL;
	
	c->name = strdup(name);
	c->args = args;
	int nbArgs = 0;
	while(*(args+nbArgs)!=NULL){
	  nbArgs++;
	}
	c->nbArgs = nbArgs;
	return c;
}

// Renvoie une commande où args vaut les éléments de arr de start
// (inclus) à end (exclus)
cmd create_command_from_array(array *arr, int start, int end){
  char **args = malloc(sizeof(char*)*(end-start+1));

  for(int i = start; i < end; ++i){
	args[i-start] = get(arr,i);
  }
	
  args[end-start] = NULL;
  return create_command(args[0], args);
}
//Concatene les tableau d'arguments
void concat_args(cmd command,char **args,int length) {
  int new_length = command->nbArgs + length + 1;
  command->args = realloc(command->args,
						  sizeof(char*) * new_length);
  for(int i = command->nbArgs; i < (new_length - 1); i++) {
	command->args[i] = args[i - command->nbArgs];
  }
  command->args[new_length - 1] = NULL;
}

int nb_args(cmd tmp){
	return tmp->nbArgs;
}

char *get_arg(int i, cmd c){
	if(i < c->nbArgs && i >= 0) return *((c->args)+i);
	else return NULL;
}

char *get_name(cmd c){
	return c->name;
}

void free_cmd(cmd c){
	free(c->name);
	for(int i = 0; c->args[i] != NULL; i++){
	  free(c->args[i]);
	}
	free(c->args);
	free(c);
}

void print_command(cmd c){
	for(int i = 0; i < c->nbArgs; ++i)
		printf("%s ", c->args[i]);
	printf("heyx \n"); 
}
