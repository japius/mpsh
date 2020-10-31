#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/lib.h"
#include "../headers/builtins.h"

int exec_line(cmdtree tree);

int exec_pipe(cmdtree left, cmdtree right);

int exec_and(cmdtree left, cmdtree right);

int exec_or(cmdtree left, cmdtree right);

int exec_semicolon(cmdtree left, cmdtree right);

int exec_commande(cmd commande);

int redir(cmdtree left, cmd right, int fd1);


int exec_line(cmdtree tree) {
  if (tree == NULL)
	return EXIT_SUCCESS;
  
  if (tree->command != NULL)
	return exec_commande(tree->command);
  
  else if(strcmp(tree->connecteur,"&&") == 0)
		return exec_and(tree->left, tree->right);

  else if(strcmp(tree->connecteur, "||") == 0)
		return exec_or(tree->left, tree->right);

  else if(strcmp(tree->connecteur, "|") == 0)
		return exec_pipe(tree->left,tree->right);

  else if(strcmp(tree->connecteur, "<") == 0
		  && tree->right->command != NULL)
		return redir(tree->left, tree->right->command, 0);

  else if(strcmp(tree->connecteur, ">") == 0
		  && tree->right->command != NULL)
		return redir(tree->left, tree->right->command, 1);
  
  else if(strcmp(tree->connecteur, "2>") == 0
		  && tree->right->command != NULL)
		return redir(tree->left, tree->right->command, 2);
  
  else if(strcmp(tree->connecteur, ";") == 0)
	return exec_semicolon(tree->left, tree->right);

  return EXIT_FAILURE;
}


int exec_pipe(cmdtree left, cmdtree right) {
  int fd[2],pid1,pid2,ret_value1,ret_value2;
  
  if (pipe(fd) != 0) {
	  perror("Error with pipe"); 
	  return EXIT_FAILURE;
  }

  if ((pid1 = fork()) == -1) {
	  perror("Error with fork");
	  return EXIT_FAILURE;
  }

  if(pid1 == 0)
	{	  
	  dup2(fd[1], 1);
	  close(fd[0]);

	  int left_status = exec_line(left);
	  exit(left_status);
	}
  
  if ((pid2 = fork()) == -1) {
	  perror("Error with fork");
	  return EXIT_FAILURE;
  }
	  
  if(pid2 == 0)
	{
	  dup2(fd[0], 0);
	  close(fd[1]);
	  
	  int right_status = exec_line(right);
		
	  exit(right_status);
	}
  
  close(fd[1]);
  close(fd[0]);

  waitpid(pid2, &ret_value2,WUNTRACED);
  waitpid(pid1, &ret_value1,WUNTRACED);
		  
  return ret_value2;  
}

  
int exec_and(cmdtree left, cmdtree right) {
	int left_status = exec_line(left);

	if(left_status != 0)
		return left_status;

	return exec_line(right);
}

int exec_or(cmdtree left, cmdtree right) {
  int left_status = exec_line(left);

  if(left_status == 0)
		return left_status;

  return exec_line(right);
}

int exec_semicolon(cmdtree left, cmdtree right) {
  int status = exec_line(left);
  if (right == NULL) {
	return status;
  }
  
  return exec_line(right);
}

int exec_commande(cmd commande) {
  char* name = commande->name;
  /*On vérifie qu'il y à au moins le nom
	de la commande dans le tableau*/
  if(name == NULL)
	return EXIT_FAILURE;  
  
  //On regarde si la commande est un builtins
  if (strcmp(name,"exit") == 0)
		return exit2(commande->args);
	
  else if (strcmp(name,"cd") == 0)
		return cd(commande->args);
  
  else if (strcmp(name,"pwd") == 0)
		return pwd(commande->args);
  
  else if (strcmp(name, "alias") == 0)
		return alias(commande->args);
  
  else if (strcmp(name, "unalias") == 0)
		return unalias(commande->args);
  
  else if (strcmp(name, "type") == 0)
		return type(commande->args);

  else if (strcmp(name, "complete") == 0)
	return complete(commande->args);

  else if (strcmp(name, "rmcomplete") == 0)
	return remove_complete(commande->args);
  
  else if (memchr(name, '=', strlen(name)) != NULL)
	return set_var(commande->args);
  
  else if (strcmp(name, "export") == 0)
	return export(commande->args);
  
  else if (strcmp(name, "unset") == 0)
	return unset_var(commande->args);
  
  else if (strcmp(name, "history") == 0) 
	return history(commande->args);

  else if (strcmp(name, "umask")==0)
	return umask2(commande->args);

  else if (strcmp(name, "?")==0) {
	printf("%d\n", last_exit_status);
	return EXIT_SUCCESS;
  }
  

  /*Si ce n'est pas un builtins :

	-Un processus fils va executer la commande
	-Le père attend que le processus fils finisse et
	récupere la valeur de retour du processus fils*/
  
  int status; 
	
  if (fork() == 0)
	{
	  char* chemin = which(name);
	  if(chemin==NULL)
		chemin=name;
	  
	  execv(chemin, commande->args);
	  
	  printf("%s: command not found \n",name);
	  free(chemin); //Si execv ne se passe pas correctement on est ici

	  exit(EXIT_FAILURE);
	}

  //Met dans status la valeur de retour
  wait(&status);

  return status;
}


int redir(cmdtree left, cmd right,int fd1) {
  int fd2;
  
  if (fd1 == 0)
		fd2 = open(right->name, O_RDONLY);
  else 
	fd2 = open(right->name, O_WRONLY | O_CREAT,
			   S_IROTH | S_IWOTH |
			   S_IRGRP | S_IWGRP |
			   S_IRUSR | S_IWUSR);
	
  if (fd2 == -1) {
		perror("");
		return EXIT_FAILURE;
  }
  
  concat_args(left->command, &(right->args[1]),
			  (right->nbArgs) - 1);
  
  if(fork() == 0)
	{
	  dup2(fd2, fd1); 
	  close(fd2);
	  int status = exec_line(left);
	  exit(status);
	}

  close(fd2);
  int status;

  wait(&status);

  return status;	
}
