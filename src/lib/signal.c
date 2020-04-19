#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>


#include "../headers/lib.h"

void nothing(int sig){
	wait(NULL);
	printf("\n");
    rl_reset_line_state();
    //rl_redisplay();
    //rl_clear_message();
    //rl_forced_update_display();
}

void init_signals(int c, char const **a){
	/*Creation d'une struct sigaction qui annule un signal*/
	struct sigaction act;
 
    act.sa_handler = nothing;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
 
 	//Signal Control+C
    sigaction(SIGINT, &act, NULL);

    //Signal Control+Z
   	//sigaction(SIGTSTP,&act,NULL);
}