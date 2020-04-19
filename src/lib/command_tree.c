#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/lib.h"

/* Renvoie une cmdcon (pointeur de command_connect) */
/* A rajouter : une liste des connecteurs autorisées*/
cmdtree new_cmdtree(cmd c, cmdtree left, cmdtree right, char *con){
	cmdtree ct = malloc(sizeof(command_tree));
	if(c != NULL){
		ct->command = c;
		ct->left = NULL;
		ct->right = NULL;
		ct->connecteur = NULL;
	} else {
		ct->command = NULL;
		ct->left = left;
		ct->right = right;
		ct->connecteur = con;
	}
	return ct;
}

/*Libère la mémoire associé à la command_connect cc
  ATTENTION : libère aussi les sous-commandes 1 et 2 */
void free_cmdtree(cmdtree ct){
  if (ct == NULL)
	return;
  
  if(ct->command != NULL){
	free_cmd(ct->command);
  } else {
	free_cmdtree(ct->left);
	free_cmdtree(ct->right);
  }
}

short is_cmd(cmdtree ct){
	if(ct == NULL) return 1;
	return (ct->command == NULL);
}

void print_cmdtree(cmdtree ct){
  if(ct == NULL) {
	printf("NULL");
	return;
  }
	
	else if(ct->command != NULL)
	  print_command(ct->command);
	else {
	  printf("LEFT :");
	  print_cmdtree(ct->left);
	  printf("\n%s\n ", ct->connecteur);
	  printf("RIGHT :");
	  print_cmdtree(ct->right);
	}
}
