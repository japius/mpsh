/*la valeur de retour de la derniere commande appeler
  on doit l'afficher quand on appuie sur '?' est appeler
  cf sujet
*/
#ifndef LIB_H
#define LIB_H

int last_exit_status;


/*-------------------------------
  STR_ARR
  ------------------------------*/
typedef struct array {
	char** tab;
	int index;
	int size;
} array;


array* init();

char *get(array *t, int index);

void clean(array* t);

void copy(char** tab1,char** tab2,int size_tab2);

void redim(array* t,int newSize); 

void add(char* str,array* t);

void print_array(array* t);

void addNULL(array* t);

/* Renvoie un tableau des sous-chaines de str
   séparé par un " " */ 
array *array_of_string(char *str);


/*-------------------------------
  COMMAND
  ------------------------------*/

typedef struct command{
	char *name; // nom de la commande
	char **args; // arguments de la commande avec args[0] la nom de la commande et NULL en dernier élément
	int nbArgs;  // nombre d'arguments de command
} command;

typedef command* cmd;

//création d'une nouvelle commande.
//args doit être de la même forme que vu ci dessus dans la structure.
//!! Attention !! ne copie pas les tableaux name et args,
// veillez à donner des tableaux avec une allocation sur.
//Renvoie NULL si l'allocation échoue
cmd create_command(char *name, char **args);

// Renvoie une commande où args vaut les éléments de arr de start
// (inclus) à end (exclus)
cmd create_command_from_array(array *arr, int start, int end);

//renvoie le nombre d'arguments de la commande tmp (temps constant)
int nb_args(cmd tmp);

//récupere l'aruments i de la commande c
//renvoie NULL si l'entier i est incorrect
char *get_arg(int i, cmd c);

//renvoie le nom de la commande c
char *get_name(cmd c);

// libere la commande c ainsi que que son tableau et tous ses sous tableaux de args
void free_cmd(cmd c);

// affiche la commande
void print_command(cmd c);



//Structure pour représenter une ligne de commande
//répertoire (src/lib)
struct command_tree{
	cmd command;
	struct command_tree *left;
	struct command_tree *right;
	char *connecteur; // ||, &&, >, <, |, etc...
};

typedef struct command_tree command_tree; 
typedef command_tree *cmdtree;

/*Renvoie NULL si l'allocation échoue,
  Ou si la première commandes est NULL
  Ou si le connecteur n'est pas dans la liste des connecteurs reconnus
 */
cmdtree new_cmdtree(cmd c, cmdtree left, cmdtree right, char *con);

/*Libère la mémoire associé à la cmdtree ct
  ATTENTION : tout les fils seront aussi libéré */
void free_cmdtree(cmdtree ct);

/* Renvoie 1 si ct->command != NULL
   Renvoie O si ct->command == NULL */
short is_cmd(cmdtree ct);

// affiche la commande correspondante à ct
void print_cmdtree(cmdtree ct);

//concatene les arguments de command avec char** args
void concat_args(cmd command,char **args,int length);



/*-------------------------------
  EXEC
  ------------------------------*/

//Execute une ligne de commande (répertoire src/lib/exec.c)
int exec_line(cmdtree tree);

//Structure pair : (key,value) utilisé dans la table de hachage
//repertoire (src/lib/pair.c)
typedef struct str_pair {
  char* key;
  char* value;
  short is_free;
} str_pair;

str_pair* new_pair(char* key, char* value);

void edit_pair(char* key, char* value, str_pair* pair);

void free_pair(str_pair* pair);

void pr_pair(str_pair* pair);


//Table de hachage (répertoire src/lib/str_hach.c)
typedef struct hach_table {
  str_pair** tab; 
  int filled_cell;
  int length;
} hach_table;

void free_table(hach_table* table);

hach_table* add_to_table(char* key, char* value, hach_table* table);

char* get_from_table(char* key, hach_table* table);

short remove_from_table(char* key, hach_table* table);

void pr_hach_table(hach_table* table);


//Initialise le signal Control+C (répertoire src/lib/signal.c)
void init_signals();
  

//auto-completion (répertoire src/lib/complete.c)
void init_autocomplete();
#endif
