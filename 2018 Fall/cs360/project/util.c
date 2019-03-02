void get_block(int dev, int blk, char *buf){
	lseek(dev, (long)(blk*BLKSIZE), 0);
	read(dev, buf, BLKSIZE);
}

void put_block(int dev, int blk, char *buf){
	lseek(dev, (long)(blk*BLKSIZE), 0);
	write(dev, buf, BLKSIZE);
}

void tokenize(char *path){
	char *temp;
	n=0;
	temp=strtok(path, "/");
	while(temp!=NULL){
		name[n]=temp;
		n++;
		temp=strtok(0, "/");	
	}
}

MINODE *iget(int dev, int ino){
	MINODE *mip = malloc(sizeof(MINODE));
	for(int i=0; i<NMINODE; i++){
		mip=&minode[i];
		if(mip->dev==dev&&mip->ino==ino){
			mip->refCount++;
			return mip;
		}
	}
	
	for(int i=0; i<NMINODE; i++){
		mip=&minode[i];
		if(mip->refCount==0){
			mip->dev=dev;
			mip->ino=ino;
			mip->refCount=1;
			char buf[BLKSIZE];
			int blk = (ino-1)/8+inode_start;
			int offset = (ino-1)%8;
			get_block(dev, blk, buf);
			INODE *ip=(INODE*)buf+offset;
			mip->INODE=*ip;
			return mip;
		}
	}
}

void iput(MINODE *mip){
	mip->refCount--;
	if(mip->refCount>0) return;
	if(!mip->dirty) return;
	int ino=mip->ino;
	char buf[BLKSIZE];
	int blk=(ino-1)/8+inode_start;
	int offset=(ino-1)%8;
	get_block(mip->dev, blk, buf);
	ip=(INODE*)buf+offset;
	*ip=mip->INODE;
	put_block(mip->dev, blk, buf);
	mip->dirty=0;
}	

int search(MINODE *mip, char *name){
	if(!S_ISDIR(mip->INODE.i_mode)){
		printf("not a directory\n");
		return 0;
	}
	char buf[BLKSIZE];
	char temp[256];
	char *cp;
	DIR *dp;
	for(int i=0; i<12; i++){
		if(mip->INODE.i_block[i]==0) return 0;
		get_block(mip->dev, mip->INODE.i_block[i], buf);
		dp=(DIR*)buf;
		cp=buf;
		while(cp<buf+BLKSIZE){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len]=0;
			if(!strcmp(name, temp)) return dp->inode;
			cp+=dp->rec_len;
			dp=(DIR*)cp;
		}
	}
	printf("does not exists\n");
	return 0;
}

int getino(char *path){
	MINODE *mip;
	if(!strcmp(path, "/")) return root->ino;
	if(path[0]=='/') mip=root;
	else mip=running->cwd;
	tokenize(path);
	int ino;
	for(int i=0; i<n; i++){
		ino=search(mip, name[i]);
		if(ino==0) return 0;
		mip=iget(dev, ino);
	}
	return ino;
}

int findmyname(MINODE *parent, u32 myino, char *myname){
	if(myino==root->ino){
		strcpy(myname, "/");
		return 1;
	}
	if(!parent) return 0;
	INODE *ip=&parent->INODE;
	char buf[BLKSIZE];
	char temp[256];
	char *cp;
	DIR *dp;
	if(!S_ISDIR(ip->i_mode)) return 0;
	for(int i=0; i<12; i++){
		if(ip->i_block[i]){
			get_block(dev, ip->i_block[i], buf);
			dp=(DIR*)buf;
			cp=buf;
			while(cp<buf+BLKSIZE){
				if(dp->inode==myino){
					strncpy(myname, dp->name, dp->name_len);
					myname[dp->name_len]=0;
					return 1;
				}
				cp+=dp->rec_len;
				dp=(DIR*)cp;
			}
		}
	}
}

int findino(MINODE *mip, u32 *myino){
	if(!mip) return 0;
	INODE *ip=&mip->INODE;
	if(!S_ISDIR(ip->i_mode)) return 0;
	char buf[BLKSIZE];
	get_block(dev, ip->i_block[0], buf);
	char *cp=buf;
	DIR *dp=(DIR*)buf;
	*myino=dp->inode;
	cp+=dp->rec_len;
	dp=(DIR*)cp;
	return dp->inode;
}














