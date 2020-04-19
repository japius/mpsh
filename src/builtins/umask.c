#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

short checkOption(char *arg,int *opt){
	for(int i=0;arg[i]!=0;i++){
		if(arg[i]=='p')
			opt[0]=1;
		else if(arg[i]=='S')
			opt[1]=1;
		else{
			fprintf(stderr, "umask: -%c: invalid option\n", arg[i]);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

mode_t getumask(void)
{
	mode_t mask = umask( 0 );
	umask(mask);
	return mask;
}

int getNum(char c){
	if(c<'0'||c>'9')
		return -1;
	return c-'0';
}

void printOpt(int *opt){
	int mask=getumask();
	short newLine=0;
	if(opt[0]==1){
		printf("umask %d%d%d ",mask>>6,(mask&56)>>3,mask&7);
		newLine=1;
	}
	if(opt[1]){
		char str[]="u=rwx,g=rwx,o=rwx";
		char *tmp=str+2;
		for(int i=1;i<=9;i++){
			if(mask&(1<<(9-i))){
				tmp[0]='-';
			}
			if(i%3==0)
				tmp=tmp+3;
			tmp++;
		}
		printf("%s",str);
		newLine=1;
	}
	if(newLine)
		printf("\n");
}

void removeRights(int newMask,int *group){
	for(int i=0;i<3;i++){
		if(group[i])
			umask(getumask()|(newMask<<(i*3)));
	}
}

void addRights(int newMask,int *group){
	for(int i=0;i<3;i++){
		if(group[i])
			umask(getumask()&(~(newMask<<(i*3))));
	}
}

void changeRights(int newMask,int *group){
	for(int i=0;i<3;i++){
		if(group[i]){
			int tmp = 7<<(i*3);
			tmp = tmp|getumask();
			tmp = tmp-(newMask<<(i*3));
			umask(tmp);
		}
	}
}

char *checkgroup(char *line, int *group){
	for(char *c=line;1;c++){
		switch (*c)
		{
			case 'u':
			group[2]=1;
			break;
			case 'g':
			group[1]=1;
			break;
			case 'o':
			group[0]=1;
			break;
			default:
			return c;
			break;
		}
	}

}

int notOctal(char *line){
	for(char *c=line;*c!=0;c++){
		int group[3]={0};
		c=checkgroup(c,group);
		void (*rightFunc)(int, int*)=NULL;
		switch (*c)
		{
			case '=':
			rightFunc= changeRights;
			break;
			case '+':
			rightFunc = addRights;
			break;
			case '-':
			rightFunc = removeRights;
			break;
			default:
			fprintf(stderr, "umask : '%c': invalid operator\n",*c);
			return EXIT_FAILURE;
			break;
		}
		c++;
		int newMask=0;
		for(;*c!=0&&*c!=',';c++){
			switch (*c)
			{
				case 'r':
				newMask=newMask|4;
				break;
				case 'w':
				newMask=newMask|2;
				break;
				case 'x':
				newMask=newMask|1;
				break;
				default:
				fprintf(stderr, "umask: '%c': invalid character\n",*c);
				return EXIT_FAILURE;
				break;
			}
		}
		rightFunc(newMask,group);
	}
	return EXIT_SUCCESS;
}

short umask2(char **args){
	int option[2] = {0};
	char *tmp = NULL;
	for(int i=1;(tmp=args[i]) !=NULL;i++){
		if(tmp[0]=='-'){
			if(checkOption(tmp+1,option)==EXIT_FAILURE){
				fprintf(stderr, "umask: usage: umask [-p] [-S] args\n");
				return EXIT_FAILURE;
			}
		}else if(getNum(tmp[0])!=-1){
			int mask=0;
			for(int i=0;tmp[i]!=0;i++){
				int val=getNum(tmp[i]);
				if(val>7||val==-1||i>=3){
					fprintf(stderr, "umask: %s: octal number out of range\n",tmp);
					return EXIT_FAILURE;
				}
				mask=mask*8+val;
			}
			umask(mask);
			break;
		}else{
			if(notOctal(tmp)==EXIT_FAILURE){
				return EXIT_FAILURE;
			}else{
				break;
			}
		}
	}
	printOpt(option);
	return EXIT_SUCCESS;
}
