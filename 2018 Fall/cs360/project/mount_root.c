void init(){
	running=malloc(sizeof(PROC));
	proc[0].pid=1;
	proc[0].uid=0;
	proc[1].pid=2;
	proc[1].uid=1;

	proc[0].cwd=0;
	proc[1].cwd=0;

	running=&proc[0];
	
	for(int i=0; i<NMINODE; i++) minode[i].refCount=0;
	root=0;	
}

void mount_root(char *device){
	char buf[BLKSIZE];
	dev=open(device, O_RDWR);
	if(dev<0){
		printf("failed to open the device\n");
		exit(0);
	}
	get_block(dev, 1, buf);
	sp=(SUPER*)buf;
	if(sp->s_magic != 0xEF53){
		printf("not applicable\n");
		exit(1);
	}
	ninodes=sp->s_inodes_count;
	nblocks=sp->s_blocks_count;
	
	get_block(dev, 2, buf);
	gp=(GD*)buf;
	
	imap=gp->bg_inode_bitmap;
	bmap=gp->bg_block_bitmap;
	inode_start=gp->bg_inode_table;

	root=iget(dev, 2);
	root->refCount=3;
	
	proc[0].cwd=root;
	proc[1].cwd=root;
}



void ls(char *path){
	MINODE *mip;
	int doput=0;
	if(!strcmp(path, "")) mip=running->cwd;
	else if(!strcmp(path, "/")) mip=root;
	else{
		if(path[0]=='/') dev=root->dev;
		else dev=running->cwd->dev;
		int ino=getino(path);
		if(ino==0){
			printf("no such pathname exists\n");
			return;
		}
		mip=iget(dev, ino);
		doput=1;
	}
	char buf[BLKSIZE];
	char temp[256];
	get_block(dev, mip->INODE.i_block[0], buf);
	char *cp=buf;
	DIR *dp=(DIR*)buf;
	while(cp<buf+BLKSIZE){
		strncpy(temp, dp->name, dp->name_len);
		temp[dp->name_len]=0;
		if(!strcmp(temp, ".")||!strcmp(temp, ".."));
		else printf("%s  ", temp);
		cp+=dp->rec_len;
		dp=(DIR*)cp;
	}	
	printf("\n");
	if(doput) iput(mip);
}

void cd(char *path){
	if(!strcmp(path, "")||!strcmp(path, "/")){
		running->cwd=root;
		return;
	}
	if(path[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;
	int ino=getino(path);
	if(ino==0){
		printf("no such path exists\n");
		return;
	}
	MINODE *temp=iget(dev, ino);
	if(!S_ISDIR(temp->INODE.i_mode)){
		printf("not accessable\n");
		iput(temp);
		return;
	}
	running->cwd=temp;
	iput(temp);
}

char *rpwd(MINODE *it){
	char temp[256];
	u32 ino;
	if(it==root){
		printf("/root");
		return;
	}
	MINODE *pmip=iget(it->dev, findino(it, &ino));
	findmyname(pmip, ino, temp);
	rpwd(pmip);
	printf("/%s", temp);
	iput(pmip);
}

void pwd(){
	if(running->cwd==root){
		printf("/root\n");
		return;
	}
	rpwd(running->cwd);
	printf("\n");
}

int quit(){
	for(int i=0; i<NMINODE; i++){
		if(minode[i].refCount>0&&minode[i].dirty){
			minode[i].refCount=1;
			iput(&minode[i]);
		}
	}
	printf("done!\n");
	exit(0);
}


