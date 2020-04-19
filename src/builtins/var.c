#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/lib.h"
#include "../headers/builtins.h"

extern char **environ;

void print_all_env() {
  for (int i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);
  }
}

void printAllVar(){
  if (var_table == NULL)
	return ;
  
  for(int i=0; i < var_table->length; i++) {
	str_pair* pair = var_table->tab[i];
	if (pair != NULL && !pair->is_free)
	  printf("%s='%s'\n",pair->key,pair->value);
  }
}

char* get_value(char *key){
  char* ret = get_from_table(key, var_table);
  if (ret != NULL)
	return ret;
  
  ret = getenv(key);
  return ret;
}

void export_aux(char* arg,short* ret) {
  char *key = strtok(arg,"=");
  char *val = strtok(NULL,"\0");
  
  if(key == NULL || (val == NULL && arg[0] == '='))
	{
	  fprintf(stderr, "export: %s : not a valid indentifier \n",arg);
	  *ret = EXIT_FAILURE;
	}
  else if (val == NULL)
	{ 
	  val = get_value(key);
		
	  if (val == NULL)
		setenv(key, "", 1);
	  else
		setenv(key, val, 1);	
	}
  else	  
	setenv(key, val, 1);
}

short export(char ** args) {
  short status = EXIT_SUCCESS;

  if (args[1] == NULL) {
	print_all_env();
	return EXIT_SUCCESS;
  }	
  
  for(int i = 1; args[i] != NULL; i++)
	export_aux(args[i], &status);

  return status;
}

void set_var_aux(char* arg, int* status) {  
  char *key = strtok(arg,"=");
  char *val = strtok(NULL,"\0");	
  
  if(key == NULL || val == NULL)
	{
	  fprintf(stderr, "%s : syntax error \n",arg);
	  *status = EXIT_FAILURE;		
	}
  else
	{	
	  var_table = add_to_table(key, val, var_table);
	  
	  if (getenv(key) != NULL) 
		setenv(key, val, 1); 
	}
}

short set_var(char ** args){
  int status = EXIT_SUCCESS;
  
  for (int i = 0; args[i] != NULL; i++)
	set_var_aux(args[i], &status);	
  
  return status;
}

short unset_aux(char *key){
  return remove_from_table(key, var_table);
}

void clearVar(){
  free_table(var_table);
}

short unset_var(char** args){
  for (int i = 1; args[i] != NULL; i++) {
	unset_aux(args[i]);
	unsetenv(args[i]);
  }
  
  return EXIT_SUCCESS;
}
