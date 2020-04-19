#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../headers/lib.h"
#include "../../parser.tab.h"

extern struct command_tree *scan_string(char const *s);

void add_to_history(char *line){
	if(*line=='\0') return;
	add_history(line);
}

void print_history(){
	int length = 0;
	for(int val = history_length; val != 0; val = val/10)
	  length++;
	
	if(length==0)
	 return;

    int i = 1;
    for(HIST_ENTRY **hist = history_list();*hist!=NULL;hist=hist+1){
	  printf("%*d %s\n",length, i,(*hist)->line);
        i++;
    }
}

char* get_line(int n){
	HIST_ENTRY *e = NULL;
	if (n >= 0)
	  e = history_get(n);

	if(e == NULL)
	  return NULL;
	
	return e->line;
}

short history(char** param){
	if(param[1] == NULL) {
		print_history();
		return EXIT_SUCCESS;
	} else {
		int n = (int) strtol(param[1], (char **) NULL, 10);
		if (n != 0) {
		  if (n > 0) {
			char *line = get_line(n);
			if (line == NULL)
			  return EXIT_FAILURE;
			cmdtree tree = scan_string(line);
			return exec_line(tree);
		  }

		  stifle_history(-n);
		  history_base = 1;
								  
		  return EXIT_SUCCESS;		  
		} else {
		  printf("unexpected parameter : should be a non null int\n");
		  return EXIT_FAILURE;
		}
	}
}
