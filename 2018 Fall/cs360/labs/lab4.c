#include <stdio.h>       // for printf()
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcpy(), strcmp(), etc.
#include <libgen.h>      // for basename(), dirname()
#include <fcntl.h>       // for open(), close(), read(), write()

// for stat syscalls
#include <sys/stat.h>
#include <unistd.h>

// for opendir, readdir syscalls
#include <sys/types.h>
#include <dirent.h>

#define buf_size 512
struct stat buf;


int main(int argc, char *argv[])
{
  if (argc < 3) printf("correct usage: mycp f1 f2\n");
  return myrcp(argv[1], argv[2]);
}

int myrcp(char *f1, char *f2)
{	
	if(!strcmp(f1, f2)){
		printf("rejected: identical objects\n");
		exit(1);
	}
	if(!(stat(f1, &buf)==0)) exit(1);
	else{
		if((!S_ISREG(buf.st_mode))*(!S_ISLNK(buf.st_mode))*(!S_ISDIR(buf.st_mode))){
			printf("rejected 1\n");
			exit(1);
		}
	} 
	if(stat(f2, &buf)==0){
		if((!S_ISREG(buf.st_mode))*(!S_ISLNK(buf.st_mode))*!S_ISDIR(buf.st_mode)){
			printf("rejected 2\n");
			exit(1);
		}
	}
	stat(f1, &buf);
	if (S_ISREG(buf.st_mode)){
        	if(!(stat(f2, &buf)==0)+S_ISREG(buf.st_mode)) return cpf2f(f1, f2);
	 	else return cpf2d(f1,f2);
      	}
	stat(f1, &buf);
	if (S_ISDIR(buf.st_mode)){
		if ((stat(f2, &buf)==0)*!S_ISDIR(buf.st_mode)){
			printf("rejected 3\n");
			exit(1);
		}
        	if (!(stat(f2, &buf)==0)) {mkdir(f2, 0766); return cpd2d(f1, f2);}
      }
}

// cp file to file
int cpf2f(char *from, char *to)
{
	int f1, f2, n;   
    char buf[buf_size];   
    struct stat old_mode;     
    // read status of the old file   
    if(stat(from, &old_mode) == -1){   
        printf("cpf2f(%s, %s), stat(%s) error!\n", from, to, from);   
        return 0;   
    }   
    // open the old file   
    if( (f1 = open(from, O_RDONLY)) == -1){   
        printf("cpf2f(%s, %s), can't open %s.\n", from, to, from);   
        return 0;   
    }   
    // create new file   
    if( (f2 = creat(to, old_mode.st_mode)) == -1){   
        printf("cpf2f(%s, %s), can't create %s.\n",  from, to, to);   
        close(f1);   
        return 0;   
    }   
    // int fchmod(int fd, mode_t mode)   
    if(fchmod(f2, old_mode.st_mode) == -1){   
        printf("cpf2f(%s, %s), fchmod(%s) error!\n", from, to, to);   
        return 0;   
    }   
    // read and write   
    while((n = read(f1, buf, BUFSIZ)) > 0){   
        if(write(f2, buf, n) != n){   
            printf("cpf2f(%s, %s), write(%s) error!\n", from, to, to);   
            close(f1);   
            close(f2);   
            return 0;   
        }   
    }   
    close(f1);   
    close(f2);   
    return 1;
}

int cpf2d(char *f1, char *f2)
{
	char *filename;
	char *file = strtok(f1, "/");
	while(file){
		filename=file;
		file=strtok(0,"/");
	}
	f2=strcat(f2, "/");
	f2=strcat(f2, filename);
	cpf2f(f1, f2);
}


int isdir(const char * path)   
{   
    struct stat bf;   
    // read status of the file   
    if(stat(path, &bf) == -1){   
        printf("isdir(%s), stat(%s) error!\n",path, path);   
        return -1;   
    }   
    if((S_IFMT & bf.st_mode) == S_IFDIR) {   
        return 1;   
    }   
    else   
        return 0;   
}   

void getfilename(char * bf, char * name)   
{   
    int i, n, j;   
    n = strlen(bf);   
    for(i = n - 1; i >=0 ; i--){   
        if(bf[i]=='/'){   
            break;   
        }   
    }   
    for(i++, j = 0; i < n; i++, j++)   
        name[j] = bf[i];   
    name[j] = '\0';   
}   
// copy a directory, including the files and sub-directories, into a directory that exists   
int cpd2d(char *f1, char *f2)   
{   
     char bf1[buf_size],  bf2[buf_size];
     char name[buf_size];   
     int flag;    
     flag = isdir(f1);   
   
     strcpy(bf1, f1);   
     strcpy(bf2, f2);   
   
     if(flag == 0) // regular file
     {   
         getfilename(bf1, name);  // get file name    
         strcat(bf2, "/");   
         strcat(bf2, name);   
             cpf2f(bf1, bf2);   
         return;   
     }   
     else    
     if(flag == 1) // directory   
     {   
        // make the same dir   
        getfilename(bf1, name);  // get dir name    
        strcat(bf2, "/");   
        strcat(bf2, name);   
  
        if(strcmp(name, ".")==0 || strcmp(name, "..")==0 ){  
            return ;
        }    
   
        if(stat(f1, &buf) == -1){   
            printf("mkdir(%s), stat(%s) error!\n", bf2, bf1);   
            return;   
        }   
   
        mkdir(bf2, buf.st_mode);
        chmod(bf2, buf.st_mode);
   
        // copy the files and subdir in the dir   
        DIR * pdir;   
        struct dirent * pdirent;   
   
        pdir = opendir(bf1);   
        while(1) {   
            pdirent = readdir(pdir) ;   
            if(pdirent == NULL)   
                break;   
            else{   
                strcpy(bf1, f1);//key   
                strcat(bf1, "/");   
                strcat(bf1, pdirent->d_name); // subfile or subdir path   
                cpd2d(bf1, bf2)  ;   // nested   
            }   
        }   
        closedir(pdir);  
        return 0;   
     }   
     else   
        return 0;   
}
