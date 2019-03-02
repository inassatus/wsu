#include "type.h"
#include "util.c"
#include "ialloc_balloc.c"
#include "mount_root.c"
#include "mkdir_creat.c"
#include "rmdir.c"
#include "links.c"
#include "misc.c"
#include "syscall.c"
#include "read_cat.c"
#include "write_cp.c"

char *cmdlist[] = {"mkdir", "rmdir", "creat", "ulink", "link", "symlink", "readlink", "stat", "chmod", "touch", "open", "close", "pfd", "read", "write", "lseek", "cat", "cp", "mv", "ls", "cd", "pwd", "quit",	"menu", NULL};

int findCmd(char *command)
{
	int i = 0;
	while(cmdlist[i]){
		if(!strcmp(command, cmdlist[i])) return i;
		// found command: return index i
		i++;
	}
	return -1;
	// not found: return -1
}

void menu(){
	int i=0;
	while(cmdlist[i]){
		printf("%d\t%s\n", i, cmdlist[i]);
		i++;
	}
}


int main(){
	init();
	mount_root("diskimage");
	while(1){
		char line[32+256+256+1];
		int index;
		printf(">");
		fgets(line, 32+256+256+1, stdin);
		line[strlen(line)-1]=0;
		sscanf(line, "%s %s %s", cmd, pathname, dest);
		int ifd=atoi(pathname);
		int ipos=atoi(dest);
		index=findCmd(cmd);
		switch(index){
			case 0: make_dir(); break;
			case 1: rmdir(); break;
			case 2: creat_file(); break;
			case 3: ulink(); break;
			case 4: link(); break;
			case 5: symlink(); break;
			case 6: readlink(); break;
			case 7: mystat(); break;
			case 8: mychmod(); break;
			case 9: touch(); break;
			case 10: open_file(); break;
			case 11: close_file(ifd); break;
			case 12: pfd(); break;
			case 13: read_file(); break;
			case 14: write_file(); break;
			case 15: lseek(ifd, ipos); break;
			case 16: cat(); break;
			case 17: mycp(); break;
			case 18: mymv(); break;
			case 19: ls(pathname); break;
			case 20: cd(pathname); break;
			case 21: pwd(); break;
			case 22: quit(); break;
			case 23: menu(); break;
			default: printf("not a valid input\n");
		}
		strcpy(cmd, "");
		strcpy(pathname, "");
		strcpy(dest, "");
	}
}
