#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include  <sys/types.h>

char *dirs[128];
int ndirs;
int narg=0;

void parsepath(char *path)
{
        char *temp;
	ndirs=0;
	temp = strtok(path, ":");  // first call to strtok()
	while(temp){
		dirs[ndirs]=temp;
		temp = strtok(0, ":");  // call strtok() until it returns NULL
		ndirs++;
	}
	return;
}

void parseline(char *line, char *arg[])
{
        char *temp;
	narg=0;
	temp = strtok(line, " ");  // first call to strtok()
	while(temp){
		arg[narg]=temp;
		temp = strtok(0, " ");  // call strtok() until it returns NULL
		narg++;
	}
	arg[narg]='\0';
	return;
}

void execute(int na, char *arg[], char *env[]){
	int isin=0;
	int isout=0;
	int isappend=0;
	char temp[64];	
	for(int i=0; i<na; i++){
		if(strcmp(arg[i],"<")==0){
			isin=i;
			break;
		}
		else if(strcmp(arg[i],">")==0){
			isout=i;
			break;
		}			
		else if(strcmp(arg[i],">>")==0){
			isappend=i;
			break;
		}
		else continue;
	}
	if(isin){
		close(0);
		for(int i=isin+1; i<na; i++){
			if(strcmp(arg[i],"<")==0|strcmp(arg[i],">")==0|strcmp(arg[i],">>")==0) continue;
			open(arg[i], O_RDONLY);
		}
		for(int i=isin; i<na; i++){
			arg[i]=NULL;
		}
	}
	if(isout){
		close(1);
		for(int i=isout+1; i<na; i++){
			if(strcmp(arg[i],"<")==0|strcmp(arg[i],">")==0|strcmp(arg[i],">>")==0) continue;
			open(arg[i], O_WRONLY|O_CREAT, 0644);
		}
		for(int i=isout; i<na; i++){
			arg[i]=NULL;
		}
	}
	if(isappend){
		close(1);
		for(int i=isappend; i<na; i++){
			if(strcmp(arg[i],"<")==0|strcmp(arg[i],">")==0|strcmp(arg[i],">>")==0) continue;
			open(arg[i], O_WRONLY|O_APPEND, 0644);
		}
		for(int i=isappend; i<na; i++){
			arg[i]=NULL;
		}
	}

		
	for(int i=0; i<ndirs; i++){
		strcpy(temp,dirs[i]);
		strcat(temp, "/");
		strcat(temp, arg[0]);
		execve(temp,arg,env);
	}
}

void pipe_execute(int bar_at, char *arg[], char *env[]){
	int pd[2];
	pipe(pd);
	char *head[64];
	int nhead=0;
	char *tail[64];
	int ntail=0;
	for(int i=0; i<bar_at; i++){
		head[nhead]=arg[i];
		nhead++;
	}
	head[nhead]='\0';
	for(int i=bar_at+1; i<narg; i++){
		tail[ntail]=arg[i];
		ntail++;
	}
	tail[ntail]='\0';

	pid_t pid=fork();

	if(pid<0) exit(0);

	if(pid==0){
		close(pd[0]);
		close(1);
		dup(pd[1]);
		close(pd[1]);
		execute(nhead, head, env);
	}
	else{
		close(pd[1]);
		close(0);
		dup(pd[0]);
		close(pd[0]);
		execute(ntail, tail, env);
	}

}

void  buildin(char *arg[], char *env[])
{
	pid_t pid=fork();
	int    status=0;	

	if(pid<0){
		exit(0);
	}
	else if(pid==0){
		int ispipe=0;
		for(int i=0; i<narg; i++){
			if(strcmp(arg[i],"|")==0) ispipe=i;
		}
		if(ispipe) pipe_execute(ispipe, arg, env);
		else execute(narg, arg, env);
	}
	else pid=wait(&status);
}

void  main(int argc, char *argv[], char *env[])
{
	char *home=getenv("HOME");
	char *path=getenv("PATH");
	char  line[1024];                      
	parsepath(path);
	while(1) {                
        	printf("mysh: ");     
        	fgets(line, 1024, stdin);
		line[strlen(line)-1] = 0;
		char  *arg[128];
		parseline(line, arg);	
		if (strcmp(arg[0], "exit") == 0) exit(0);     
		else if(strcmp(arg[0],"cd")==0){
			if(strlen(arg[1])>0) chdir(arg[1]);
			else chdir(home);
			continue;
		}
      		else buildin(arg, env);
	}
}
