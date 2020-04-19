#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/lib.h"

array* init() {
  array* t = malloc(sizeof(array));
  
  if (t == NULL)
    return NULL;
  
  t->index = 0;
  t->size = 1024;
  t->tab = malloc(sizeof(char*) * t->size);

  if (t->tab == NULL) {
    free(t);
    return NULL;
  }

  return t;
}

char *get(array *t, int i){
	if(i < 0 || i > t->size){
		return NULL;
	} else {
		return t->tab[i];
	}
}

void clean(array* t) {
  if (t->tab != NULL) {
	if (t->tab != NULL) {
	  for(int i = 0 ; i < t->index; i++) {
		free(t->tab[i]);
	  }
	  free(t->tab);
	}
	free(t);
  }
}

void redim(array* t,int newSize) {
  t->tab = realloc(t->tab, sizeof(char*) * newSize);   
  t->size = newSize;
}

void add(char* str,array* t) {
  if (t->index >= t->size)
	redim(t, 2 * t->size);
  
  t->tab[t->index] = strdup(str);
  t->index = (t->index) + 1;
}

void print_array(array* t) {
  for (int i=0; i < t->index; i++) {
	printf("%s\n ", t->tab[i]);
  }
  printf("\n");
}

void addNULL(array* t) {
  if (t->index >= t->size)
	redim(t, 2 * t->size);
  
  t->tab[t->index] = NULL;
  t->index = (t->index) + 1;
}

/* Renvoie un tableau des sous-chaines de str
   séparé par un " " */ 
array *array_of_string(char *str){
	array *arr = init();
	char delim[2] = " ";
	char *token = strtok(str,delim);
	while(token != NULL) {
		add(token, arr);
		token = strtok(NULL, delim);
	}
	return arr;
}
