#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <regex.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "../headers/lib.h"
#include "../headers/builtins.h"

static int sizeoftext;


static char* path;
static char* token;

static DIR *directory;

//Fonctionne bien
char *complete_from_path(const char *text){	
  if(directory == NULL){
	if(token == NULL)
	  return NULL;
	
	directory = opendir(token);
	token=strtok(NULL, ":");
	if(directory==0)
	  return complete_from_path(text);
  }
  
  struct dirent *entree;

  if((entree = readdir(directory)) == NULL){
	closedir(directory);
	directory = NULL;
	return complete_from_path(text);
  }
  
  if(strncmp(text,entree->d_name,sizeoftext) == 0){
	return strdup(entree->d_name);
  }
  
  return complete_from_path(text);
}

static const char* internals[] ={"exit","cd","pwd","alias","unalias",
								 "export","umask","history",
								 "complete","rmcomplete","type",NULL};
static int current = 0;

char *complete_as_internal(const char* text){
	if(internals[current]==NULL){
		return NULL;
	}
	else if(strncmp(internals[current],text,sizeoftext)==0){
		current++;
		return strdup(internals[current-1]);
	}
	current++;
	return complete_as_internal(text);
}

static int count_alias = 0;

char *complete_as_alias(const char* text){
  if (alias_table == NULL)
	return NULL;
  
  if(count_alias == alias_table->length){
	return NULL;
  }

	str_pair* pair = alias_table->tab[count_alias];

	count_alias++;

	if (pair != NULL && !pair->is_free
		&& strncmp(pair->key, text, sizeoftext) == 0)
	  return strdup(pair->key);

	return complete_as_alias(text);
}


char *complete_as_cmd(const char *text, int state){
	if(!state){
		path  = strdup(getenv("CHEMIN"));
		token = strtok(path, ":");
		count_alias=0;
		current=0;
 	}

	char* res = NULL;
	if((res = complete_from_path(text)))
	  return res;
	else if((res = complete_as_internal(text)))
	  return res;
	else
	  return complete_as_alias(text);
}

static int count_var=0;

char *complete_as_var(const char* text,int state){
	if(state==0){
		count_var=0;
	}
	if (var_table == NULL)
		return NULL;
  
  	if(count_var == var_table->length){
		return NULL;
  	}

	str_pair* pair = var_table->tab[count_var];

	count_var++;

	if (pair != NULL && !pair->is_free
		&& strncmp(pair->key, text, sizeoftext) == 0)
	  return strdup(pair->key);

	return complete_as_var(text,1);
}

short is_operator(char c) {
  return c == '|' || c == '&' || c == ';'
	|| c == '\n';
}

short is_command() {
  short is_comm = 1;
  char* line = rl_line_buffer;

  for (int i = 0; line[i] != '\0'; i++) {
	if (line[i] != ' ')
	  {
		if (i == rl_point - sizeoftext)		 
		  return is_comm;
		else if (is_operator(line[i]))
		  is_comm = 1;
		else
		  is_comm = 0;
	  }
	
  }
  
  return is_comm;  
}

char* get_last_complete_query() {
  short is_comm = 1;
  char* line = strndup(rl_line_buffer, rl_point - sizeoftext);
  
  char* tok = strtok(line, " ");
  char* ret = NULL;
  
  while(tok != NULL) {		
	if (is_comm)
	  ret = get_complete(tok);
	
	is_comm = 0;

	
	if (is_operator(tok[0]))
	  is_comm = 1;
	
	tok = strtok(NULL, " ");
  }
  
  return ret;  
}

short match_exp(char* regexp,char* str,int taille) {
  regex_t regexCompiled;
  regmatch_t match[1];
  
  if (regcomp(&regexCompiled, regexp, REG_EXTENDED))
	return 0;

  if (regexec(&regexCompiled, str, 1, match, 0))
	return 0;  // No more matches
  
  if (match->rm_so == (size_t)-1)
	return 0;  // No more groups*/

  short tmp = match[0].rm_so == 0 && match[0].rm_eo == taille;
  regfree(&regexCompiled);
  
  return tmp;
}

void get_chemin_str(char chemin[],char* str) {
  	chemin[0] = '\0';
	char* courant = getPath();

	strcat(chemin, courant);
	strcat(chemin, "/");
	strcat(chemin, str);
	free(courant);
}

short is_directory(const char* chemin) {
  struct stat stats;
  if (stat(chemin, &stats) != 0)
	return 0;

  return S_ISDIR(stats.st_mode);
}

char* complete_filename(const char *text, int state) {
  char* str = rl_filename_completion_function(text, state);

  if (str == NULL)
	return NULL;

  char* restriction = get_last_complete_query();


  if (restriction == NULL)
	return str;
  
  else if (strcmp(restriction, "directory") == 0) {
	char chemin[PATH_MAX + 1];
	get_chemin_str(chemin,str);

	if (is_directory(chemin))
	  return str;
	else
	  return complete_filename(text, 1);
  }

  
  if (match_exp(restriction, str, strlen(str)))
	return str;
  else
	return complete_filename(text, 1);  
}


char **my_completion(const char *text, int start, int end){
	rl_attempted_completion_over = 0;
	sizeoftext = end-start;
	if(rl_line_buffer[start-1]=='$'){
		rl_attempted_completion_over = 1;
		return rl_completion_matches(text,complete_as_var);
	}

	if (is_command()) {
	  return rl_completion_matches(text,complete_as_cmd);
	}
	else
	  return rl_completion_matches(text, complete_filename);
}

void init_autocomplete(){
  rl_attempted_completion_function = my_completion;
}
