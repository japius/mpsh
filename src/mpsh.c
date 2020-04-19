#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <linux/limits.h>

#include "headers/builtins.h"
#include "headers/lib.h"
#include "../parser.tab.h"

extern struct command_tree *scan_string(char const *s);

void preprocessing(char* lines);

void init_variable_chemin();

short read_from_file(const char* filename);

char *get_invite();



short read_mpshrc() {
  char path[PATH_MAX + 1];
  strcat(path, getenv("HOME"));
  strcat(path, "/.mpshrc");

  return read_from_file(path); 
}

//Quelques tests
int main(int argc, char const *argv[]) {

	last_exit_status = 0; //défini dans headers/lib.h
  	alias_table=NULL;
  	var_table=NULL;

  init_variable_chemin(); //défini ci-dessous
  read_mpshrc();
  complete_aux("cd", "directory");
	
  init_signals(); //défini dans lib/signals
  init_autocomplete(); //défini dans lib/complete



  if(argc == 1) {
    while (1) {
      char *prompt = get_invite();
      char* line = readline(prompt);
	  

      if (line == NULL)
	break;

      add_to_history(line);
	  
      cmdtree tree = scan_string(line);

      last_exit_status = exec_line(tree);
	  
      free_cmdtree(tree);
      free(line);
      free(prompt);
    }
  } else {
    exit(read_from_file(argv[1]));
  }

  return EXIT_SUCCESS;
}


void init_variable_chemin() {
  char* chemin = strdup(getenv("PATH"));
  setenv("CHEMIN", chemin, 1);
}



short read_from_file(const char* filename) {
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  
  FILE *fp = fopen(filename, "r");
  
  if (fp == NULL)
    return EXIT_FAILURE;
  
  while ((read = getline(&line, &len, fp)) != -1) {
    cmdtree tree = scan_string(line);
    last_exit_status = exec_line(tree);
	
    free_cmdtree(tree);
  }
  
  fclose(fp);
  free(line);
	
  return EXIT_SUCCESS;		
}

char *get_invite() {
  char *format = get_value("INVITE");
  if (format == NULL) {
    format = ":|w -> ";
  } 
  char *invite = malloc(sizeof(char)*(PATH_MAX + 1));
  invite[0]='\0';
  int pos = 0;
  int i = 0;
  while(format[i] != '\0'){
    if (format[i] == '|') {
      if (format[i+1] == 'w') {
	char *chemin = getPath();
	strcat(invite,chemin);
	free(chemin);
	pos = strlen(invite);
	i++;
      } else if(format[i+1] == 'W') {
	char *chemin = getPath();
	char *tok = chemin;
	char *dir = tok;
	while((tok = strtok(tok,"/")) != NULL){
	  dir = tok;
	  tok = NULL;
	}
	strcat(invite,dir);
	free(chemin);
	pos = strlen(invite);
	i++;
      } else if(format[i+1] == 'u') {
	char *user = strdup(getenv("USER"));
	strcat(invite,user);
	free(user);
	pos = strlen(invite);
	i++;
      } else {
	invite[pos] = format[i];
	pos++;
	invite[pos] = '\0';
      }
    } else {
      invite[pos] = format[i];
      pos++;
      invite[pos] = '\0';
    }
    i++;
  }
  return invite;
}
