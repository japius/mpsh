#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/lib.h"

str_pair* new_pair(char* key, char* value) {
  str_pair* pair = malloc(sizeof(str_pair));

  if (pair == NULL)
	return NULL;
  
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->is_free = 0;

  if (pair->key == NULL || pair->value == NULL) {
	free(pair->key);
	free(pair->value);
	free(pair);
	return NULL;
  }
  
  return pair;
}

void edit_pair(char* key, char* value, str_pair* pair) {
  free(pair->value);
  free(pair->key);
  
  pair->value = strdup(value);
  pair->key = strdup(key);
  pair->is_free = 0;
}

void free_pair(str_pair* pair) {
  if (pair == NULL)
	return;
  
  free(pair->key);
  free(pair->value);
  free(pair);
}

void pr_pair(str_pair* pair) {
  if (pair != NULL && !pair->is_free)
	printf("(KEY,VALUE) = (%s,%s) \n",pair->key,pair->value);
  else
	printf("FREE \n");
}
