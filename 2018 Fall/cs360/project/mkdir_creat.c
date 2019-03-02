int enter_name(MINODE *pmip, int myino, char *myname){
	INODE *pip=&pmip->INODE;
	char buf[BLKSIZE];
	char *cp;
	DIR *dp;

	int need_len=4*((8+strlen(myname)+3)/4);
	int ideal_len=0;
	int remain=0;
	int bno=0;

	for(int i=0; i<12; i++){
		if(pip->i_block[i]==0){
			bno=balloc(dev);
			pip->i_block[i]=bno;
			get_block(dev, bno, buf);
			pip->i_size+=BLKSIZE;
			cp=buf;
			dp=(DIR*)buf;
			dp->inode=myino;
			dp->rec_len=BLKSIZE;
			dp->name_len=strlen(myname);
			strcpy(dp->name, myname);
			put_block(dev, bno, buf);
			return 1;			
		}
		bno=pip->i_block[i];
		get_block(dev, bno, buf);
		cp=buf;
		dp=(DIR*)buf;
		
		while(cp+dp->rec_len<buf+BLKSIZE){
			cp+=dp->rec_len;
			dp=(DIR*)cp;
		}
		ideal_len=4*((8+dp->name_len+3)/4);
		remain=dp->rec_len-ideal_len;
		if(remain>=need_len){
			dp->rec_len=ideal_len;
			cp+=dp->rec_len;
			dp=(DIR*)cp;
			dp->inode=myino;
			dp->rec_len=BLKSIZE-((u32)cp-(u32)buf);
			dp->name_len=strlen(myname);
			strcpy(dp->name, myname);
			put_block(dev, bno, buf);
			return 1;
		}
	}
	printf("implemented only 12 direct blocks, no more entry can be added\n");
}

int mymkdir(MINODE *pip, char *name){
	int ino=ialloc(dev);
	int bno=balloc(dev);
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	char buf[BLKSIZE];

	ip->i_mode=0x41ED;
	ip->i_uid=running->uid;
	ip->i_gid=running->gid;
	ip->i_size=BLKSIZE;
	ip->i_links_count=2;
	ip->i_atime=time(0L);
	ip->i_ctime=time(0L);
	ip->i_mtime=time(0L);
	ip->i_blocks=2;
	ip->i_block[0]=bno;
	for(int i=1; i<15; i++) ip->i_block[i]=0;
	mip->dirty=1;
	iput(mip);

	get_block(dev, bno, buf);
	DIR *dp=(DIR*)buf;
	char *cp=buf;

	dp->inode=ino;
	strncpy(dp->name, ".", 1);
	dp->name_len=1;
	dp->rec_len=12;
	
	cp+=dp->rec_len;
	dp=(DIR*)cp;
	
	dp->inode=pip->ino;
	strncpy(dp->name, "..", 2);
	dp->name_len=2;
	dp->rec_len=1012;
	
	put_block(pip->dev, bno, buf);
	enter_name(pip, ino, name);
	
	return 1;
}

void make_dir(){
	if(!strcmp(pathname, "")) return;
	char temp1[256], temp2[256];
	char buf[BLKSIZE];
	char dir[256], base[256];

	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	strcpy(temp1, pathname);
	strcpy(temp2, pathname);
	strcpy(dir, dirname(temp1));
	strcpy(base, basename(temp2));
	
	int pino=getino(dir);
	MINODE *pmip=iget(dev, pino);
	INODE *pip=&pmip->INODE;
	if(!pmip) return;
	if(!S_ISDIR(pip->i_mode)) return;
	if(getino(pathname)!=0){
		printf("already exists\n");
		return;
	}
	mymkdir(pmip, base);
	
	pip->i_links_count++;
	pip->i_atime=time(0L);
	pmip->dirty=1;
	iput(pmip);
}

int my_creat(MINODE *pmip, char *name){
	int ino=ialloc(dev);
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	
	ip->i_mode=0x81A4;
	ip->i_uid=running->uid;
	ip->i_gid=running->gid;
	ip->i_size=0;
	ip->i_links_count=1;
	ip->i_atime=time(0L);
	ip->i_ctime=time(0L);
	ip->i_mtime=time(0L);
	ip->i_blocks=0;
	for(int i=0; i<14; i++) ip->i_block[i]=0;
	mip->dirty=1;
	iput(mip);
	enter_name(pmip, ino, name);
	
	return ino;
}

int creat_file(){
	char buf[BLKSIZE];
	char temp1[256], temp2[256];
	char dir[256], base[256];

	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;
	
	strcpy(temp1, pathname);
	strcpy(temp2, pathname);
	strcpy(dir, dirname(temp1));
	strcpy(base, basename(temp2));

	int pino=getino(dir);
	MINODE *pmip=iget(dev, pino);
	INODE *pip=&pmip->INODE;

	if(!pmip) return;
	if(!S_ISDIR(pip->i_mode)) return;
	if(getino(pathname)!=0) return;

	int ino=my_creat(pmip, base);
	pip->i_atime=time(0L);
	pmip->dirty=1;
	iput(pmip);
	return ino;
}















