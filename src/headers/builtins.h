#ifndef BUILTINS_H
#define BUILTINS_H

#include "lib.h"

/* CD functions */
short cd(char** args);

/*PWD functions*/
short pwd(char** args);

char* getPath();

/*exit function*/
short exit2(char** args);

/*alias function*/
// Affiche la liste des alias si n'a aucun arguments

hach_table* alias_table;


short alias(char ** args);

//Renvoie l'alias associé a key
//NULL sinon
char *get_alias(char *key);

/*unalias function*/
// Supprime tous les alias si n'a aucun arguments
short unalias(char ** args);

//Ajoute line a l'hitorique (une copie) sauf si line est vide
void add_to_history(char *line);

/* history function */
//Affiche la liste de tout l'historique
void print_history();

//Revoie la la n-eme ligne de l'historique (0 le plus ancien)
//Si n négatif revoie la n-eme ligne
//en commencant par la fin (-1 la derniere ligne ajouté)
char *get_line(int n);

/*umask function*/
short umask2(char **args);

short history(char ** args);

short type(char** args);

char* which(const char* filename);

short export(char ** args);

hach_table* var_table;

//variable
short set_var(char ** args);

short unset_var(char** args);

char* get_value(char *key);

//REGEXP AUTOCOMPLETE

void print_regexp_table();

char* get_complete(char *key);

short complete_aux(char* key, char* value);

short complete (char ** args);

short remove_complete(char** args);

#endif
