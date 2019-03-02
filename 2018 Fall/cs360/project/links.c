int link(){
	char temp1[256], temp2[256];
	char dir[256], base[256];

	if(!strcmp(pathname, "")) return 0;
	if(!strcmp(dest, "")) return 0;

	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	int ino=getino(pathname);
	if(ino==0){
		printf("no such file");
		return 0;
	}
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	if(!mip){
		printf("could not link\n");
		iput(mip);
		return 0;
	}
	if(S_ISDIR(mip->INODE.i_mode)){
		printf("it is a directory\n");
		iput(mip);
		return 0;
	}

	strcpy(temp1, dest);
	strcpy(dir, dirname(temp1));
	strcpy(temp2, dest);
	strcpy(base, basename(temp2));

	int pino=getino(dir);
	if(pino==0){
		printf("could not link\n");
		iput(mip);
		return 0;
	}
	MINODE *pmip=iget(dev, pino);
	INODE *pip=&pmip->INODE;

	if(!pmip){
		printf("could not link\n");
		iput(mip);
		iput(pmip);
		return 0;
	}
	if(!S_ISDIR(pmip->INODE.i_mode)){
		printf("could not link\n");
		iput(mip);
		iput(pmip);
		return 0;
	}
	if(getino(dest)){
		printf("already exists\n");
		iput(mip);
		iput(pmip);
		return 0;
	}
	enter_name(pmip, ino, base);
	ip->i_links_count++;
	pip->i_atime=time(0L);
	mip->dirty=1;
	pmip->dirty=1;
	iput(mip);
	iput(pmip);
	return 1;
}

void ulink(){

	char temp1[256], temp2[256];
	char dir[256], base[256];

	if(!strcmp(pathname, "")) return;
	if(!strcmp(pathname, "/")) return;
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	strcpy(temp1, pathname);
	strcpy(temp2, pathname);
	strcpy(dir, dirname(temp1));
	strcpy(base, basename(temp2));

	int ino=getino(pathname);
	if(ino==0){
		printf("no such path\n");
		return;
	}

	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	if(!mip){
		printf("could not ulink");
		iput(mip);
		return;
	}
	if(S_ISDIR(ip->i_mode)){
		printf("it is a directory\n");
		iput(mip);
		return;
	}
	ip->i_links_count--;
	if(ip->i_links_count==0){
		if(!((ip->i_mode&0120000)==0120000)) truncate(ip);
	}
	idealloc(dev, ino);

	int pino=getino(dir);
	MINODE *pmip=iget(dev, pino);
	INODE *pip=&pmip->INODE;
	rm_child(pmip, base);
	pip->i_atime=time(0L);
	pip->i_mtime=time(0L);
	mip->dirty=1;
	pmip->dirty=1;
	iput(mip);
	iput(pmip);
}

void symlink(){
	char temp[256];
	char old[256]; 

	if(!strcmp(pathname, "")) return;
	if(!strcmp(dest, "")) return;
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;
	
	strcpy(temp, pathname);
	strcpy(old, basename(temp));

	int ino=getino(pathname);
	if(ino==0){
		printf("no such path\n");
		return;
	}
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	if(!mip){
		printf("does not exists\n");
		iput(mip);
		return;
	}
	strcpy(pathname, dest);
	int lino=creat_file();
	if(!lino){
		printf("error: check the directory\n");
		iput(mip);
		return;
	}
	MINODE *lmip=iget(dev, lino);
	INODE *lip=&lmip->INODE;

	lip->i_mode=0120000;
	strcpy((char*)lip->i_block, old);
	lip->i_size=strlen(old);
	lmip->dirty=1;
	iput(mip);
	iput(lmip);
}

void readlink(){
	if(!strcmp(pathname, "")){
		printf("no pathname given\n");
		return;
	}
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	int ino=getino(pathname);
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	
	if(ip->i_mode!=0120000){
		printf("not a sym lnk file \n");
		iput(mip);
		return;
	}
	printf("%s", ip->i_block);
	printf("\n");
}



