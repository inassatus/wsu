#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
          char  nodename[64];       // node's name string
          char  type;
   	  struct node *child, *sibling, *parent;
}NODE;


NODE *root, *cwd, *start;
char line[128];                   // for gettting user input line
char command[64], pathname[64];   // for command and pathname strings
char dname[64], bname[64];
char *name[100];                  // token string pointers 
int  n;                           // number of token strings in pathname 
FILE *fp;
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", "save", "menu", "quit", NULL};

void initialize(){
	NODE *new=(NODE*)malloc(sizeof(struct node));
	strcpy(new->nodename, "home");
	new->type='D';
	new->child=NULL;
	new->sibling=NULL;
	new->parent=NULL;
	root=new;
	cwd=root;
	return;
}

int tokenize(char *pathname)
{
    	char *s;
	n=0;
	s = strtok(pathname, "/");  // first call to strtok()
	while(s){
		name[n]=s;
		s = strtok(0, "/");  // call strtok() until it returns NULL
		n++;
	}
	return n;
}

NODE *search_child(NODE *parent, char *name)
{ 
	NODE *instant = parent->child;
	while(instant){
		if(!strcmp(instant->nodename,name)) return instant;
		instant=instant->sibling;
	}
	return 0;
}

NODE *path2node(char *pathname)
{ 

   if (pathname[0]=='/') start = root;
   else start = cwd;

   n = tokenize(pathname); // NOTE: pass a copy of pathname
   NODE *p = start;

   for (int i=0; i < n; i++){
       p = search_child(p, name[i]);
       if (p==0) return 0;            // if name[i] does not exist
   }
   return p;
}

int findCmd(char *command)
{
	int i = 0;
	while(cmd[i]){
		if(!strcmp(command, cmd[i])) return i;
		// found command: return index i
		i++;
	}
	return -1;
	// not found: return -1
}
void mkdir(char *pathname){
	if(path2node(pathname)){
		printf("this directory already exists\n");
	}
	else if(cwd->child){
		NODE *new=(NODE*)malloc(sizeof(struct node));
		strcpy(new->nodename, name[n-1]);
		new->type='D';
		new->child=NULL;
		char dir[64]="/";
		for(int i=0; i<n-1; i++){
			strcat(dir, name[i]);
			strcat(dir, "/");
		}
		NODE *temp=path2node(dir);
		if(temp){
			new->sibling=temp->child;
			new->parent=temp;
			temp->child=new;
		}
		else printf("directory does not exists\n");
	}
	else{
		NODE *new=(NODE*)malloc(sizeof(struct node));
		strcpy(new->nodename, name[n-1]);
		new->type='D';
		new->child=NULL;
		new->sibling=NULL;
		char dir[64]="/";
		for(int i=0; i<n-1; i++){
			strcat(dir, name[i]);
			strcat(dir, "/");
		}
		NODE *temp=path2node(dir);
		if(temp){
			new->sibling=temp->child;
			new->parent=temp;
			temp->child=new;
		}
		else printf("directory does not exists\n");
	}
	return;
}
void rmdir(char *pathname){
	NODE *temp = path2node(pathname);
	if(temp){
		if(temp->type=='D'){
			if(temp->parent->child->nodename==temp->nodename){
				if(temp->sibling) temp->parent->child=temp->sibling;
				temp->parent->child=NULL;		
			}//remove point from parent
			NODE *instant = temp->parent->child;
			while(instant){
				if(instant->sibling->nodename==temp->nodename){
					if(temp->sibling) instant->sibling=temp->sibling;
					else instant->sibling=NULL;		
				}
				instant=instant->sibling;
			}//remove point from sibling
			temp=NULL;
		}
		else printf("it is not a directory\n");
	}
	else printf("there is no such directory\n");
	return;
}
void ls(){
	NODE *instant = cwd->child;
	while(instant){
		printf("%s ",instant->nodename);
		instant=instant->sibling;
	}
	printf("\n");
	return;
}
void cd(char *pathname){
	if(!strcmp(pathname,"...")){
		if(cwd->parent) cwd=cwd->parent;
		else printf("there is no upper directory\n");
		return;
	}	
	NODE *temp = path2node(pathname);
	if(temp){
		if(temp->type=='D') cwd=temp;
		else printf("this is not a directory\n");
	}
	else printf("there is no such directory\n");
	return;
}
void pwd(){
	char path[64];
	strcpy(path,"");
	while((strcmp(cwd->nodename, "home"))){
		char temp[64];
		strcpy(temp, cwd->nodename);
		strcat(temp, path);
		strcpy(path, temp);
		strcpy(temp, "/");
		strcat(temp, path);
		strcpy(path, temp);
		cwd=cwd->parent;
	}
	printf("%s\n",path);
	cd(path);
	return;
}
void creat(char *pathname){
	pathname=strtok(pathname,".f");
	if(path2node(strcat(pathname,".f"))){
		printf("this file already exists\n");
	}
	else if(cwd->child){
		NODE *new=(NODE*)malloc(sizeof(struct node));
		strcpy(new->nodename, name[n-1]);
		new->type='F';
		new->child=NULL;
		char dir[64]="/";
		for(int i=0; i<n-1; i++){
			strcat(dir, name[i]);
			strcat(dir, "/");
		}
		NODE *temp=path2node(dir);
		if(temp){
			new->sibling=temp->child;
			new->parent=temp;
			temp->child=new;
		}
		else printf("directory does not exists\n");
	}
	else{
		NODE *new=(NODE*)malloc(sizeof(struct node));
		strcpy(new->nodename, name[n-1]);
		new->type='F';
		new->child=NULL;
		new->sibling=NULL;
		char dir[64]="/";
		for(int i=0; i<n-1; i++){
			strcat(dir, name[i]);
			strcat(dir, "/");
		}
		NODE *temp=path2node(dir);
		if(temp){
			new->sibling=temp->child;
			new->parent=temp;
			temp->child=new;
		}
		else printf("directory does not exists\n");
	}
	return;
}
void rm(char *pathname){
	pathname=strtok(pathname,".f");
	NODE *temp = path2node(strcat(pathname,".f"));
	if(temp){
		if(temp->type=='F'){
			if(temp->parent->child->nodename==temp->nodename){
				if(temp->sibling) temp->parent->child=temp->sibling;
				else temp->parent->child=NULL;		
			}//remove point from parent
			NODE *instant = temp->parent->child;
			while(instant){
				if(instant->sibling->nodename==temp->nodename){
					if(temp->sibling) instant->sibling=temp->sibling;
					else instant->sibling=NULL;		
				}
				instant=instant->sibling;
			}//remove point from sibling
			temp=NULL;
		}
	}
	else printf("there is no such File\n");
	return;
}
void save(char *pathname){
	pathname=strtok(pathname,".f");
	NODE *temp = path2node(strcat(pathname,".f"));
	if(!temp){
		printf("no existing file to save: attempt to create\n");
		creat(pathname);
		temp=path2node(pathname);
	}
	if(temp){
		printf("successfully created\n");
		temp->type='T';
		//indicate binary tree is saved
	}	
	return;
}

void printnodes(NODE *x){
	if(x){
		NODE *looping=x;
		char path[64];
		strcpy(path,"");
		while((strcmp(looping->nodename, "home"))){
			char temp[64];
			strcpy(temp, looping->nodename);
			strcat(temp, path);
			strcpy(path, temp);
			strcpy(temp, "/");
			strcat(temp, path);
			strcpy(path, temp);
			looping=looping->parent;
			}
		printf("%c:	%s\n",x->type,path);
		printnodes(x->child);
		printnodes(x->sibling);
	}
	return;
}
void reload(char *pathname){
	pathname=strtok(pathname,".f");    
	NODE *temp = path2node(strcat(pathname,".f"));
	if(temp){
		if(temp->type=='T') printnodes(root);
	}
	else printf("there is no such file");
	return;
}
void menu(){
	printf("Available Commands\n");
	for(int i=0; i<11; i++){
		printf("%d:	%s\n",findCmd(cmd[i]) , cmd[i]);
	}
	return;
}

int main()
{
	int index;
	char line[128], command[16], pathname[64];
	initialize();   
	//initialize root node of the file system tree
	while(1){
		printf("input a commad line : ");
		fgets(line,128,stdin);
		line[strlen(line)-1] = 0;
		sscanf(line, "%s %s", command, pathname);
		index = findCmd(command);
		switch(index){ 
			case 0 : mkdir(pathname);    break;
			case 1 : rmdir(pathname);    break;
			case 2 : ls();       break;
			case 3: cd(pathname); break;
			case 4: pwd(); break;
			case 5: creat(pathname); break;
			case 6: rm(pathname); break;
			case 7: reload(pathname); break;
			case 8: save(pathname); break;
			case 9: menu(); break;
			case 10: return 0;
			default: printf("invalid command %s\n", command);
		}
	}
}


































