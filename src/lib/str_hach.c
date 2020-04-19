#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/lib.h"

#define STARTING_LENGTH 256
#define JUMP_SIZE 13


hach_table* new_hach_table(int length) {
  hach_table* table = malloc(sizeof(hach_table));

  if (table == NULL)
	return NULL;

  table->tab = malloc(sizeof(str_pair*) * length);
  table->length = length;
  table->filled_cell = 0;

  if (table->tab == NULL) {
	free(table);
	return NULL;
  }

  for(int i=0; i < table->length; i++) {
	table->tab[i] = NULL;
  }

  return table;
}

hach_table* init_table() {
  return new_hach_table(STARTING_LENGTH);
}

void free_table(hach_table* table) {
  if (table == NULL)
	return;
  
  for(int i = 0; i < table->length; i++) {
	free_pair(table->tab[i]);
  }
  
  free(table->tab);
  free(table);
}

hach_table* resize_table(hach_table* table) {

  hach_table* new_table = new_hach_table(2 * table->length);
  
  for (int i = 0; i < table->length ; i++) {
	if (table->tab[i] != NULL && !table->tab[i]->is_free) {
	  char* key = table->tab[i]->key;
	  char* value = table->tab[i]->value;
	  new_table = add_to_table(key, value, new_table);
	}
  }

  free_table(table);

  return new_table;
}

int hach_function(char* key,int length) {
  int acc = 0;
  
  for(int i=0; key[i] != '\0'; i++) {
	acc += key[i];
  }
  
  return acc % length;
}

float filled_rate(hach_table* table) {
  return ((float) table->filled_cell + 1) / ((float) table->length);
}

short cell_empty(int pos, char* key, hach_table* table) {
  return table->tab[pos] == NULL
	|| table->tab[pos]->is_free
	|| strcmp(table->tab[pos]->key, key) == 0;
}

void add_to_cell(int pos,char* key, char* value, hach_table* table) {
  table->filled_cell = table->filled_cell + 1;
  if (table->tab[pos] == NULL)
	table->tab[pos] = new_pair(key, value);
  else
	edit_pair(key, value, table->tab[pos]);
}

hach_table* add_to_table(char* key, char* value, hach_table* table) {
  if (table == NULL)
	table = init_table();
  
  if(filled_rate(table) >= 0.5)
	table = resize_table(table);
  
  int pos = hach_function(key, table->length);

  if (cell_empty(pos,key,table)) {
	add_to_cell(pos, key, value, table);
	return table;
  }
  
  for (int i = pos + JUMP_SIZE; (i % table->length) != pos; i += JUMP_SIZE ) {	
	if (cell_empty((i % table->length), key, table)) {
	  add_to_cell((i % table->length), key, value, table);
	  return table;
	}
  }

  return table;
}

short is_key_at_pos(int pos, char* key, hach_table* table) {
  return table->tab[pos] != NULL
	&& strcmp(key, table->tab[pos]->key) == 0
	&& !table->tab[pos]->is_free;
}

char* get_from_table(char* key, hach_table* table) {
  if (table == NULL)
	return NULL;
  
  int pos = hach_function(key, table->length);

  if (is_key_at_pos(pos, key, table)) {
	return table->tab[pos]->value;
  }
  
  for (int i = pos + JUMP_SIZE; (i % table->length) != pos; i += JUMP_SIZE ) {
	if (is_key_at_pos(i % table->length, key, table))
	  return table->tab[i % table->length]->value;
  }

  return NULL;
}

short remove_from_table(char* key, hach_table* table) {
  if (table == NULL)
	return 0;
  
  int pos = hach_function(key, table->length);

  if (is_key_at_pos(pos, key, table)) {
	table->tab[pos]->is_free = 1;
	return 1;
  }
  
  for (int i = pos + JUMP_SIZE; (i % table->length) != pos; i += JUMP_SIZE ) {
	if (is_key_at_pos(i % table->length, key, table)) {
	  table->tab[i % table->length]->is_free = 1;
	  return 1;
	}
  }

  return 0;
}

void pr_hach_table(hach_table* table) {
  if (table == NULL)
	return;
  
  for(int i=0; i < table->length; i++) {
	pr_pair(table->tab[i]);
  }
}
