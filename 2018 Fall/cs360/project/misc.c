void mystat(){
	struct stat myst;
	if(!strcmp(pathname, "")){
		printf("no pathname is given\n");
		return;
	}
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	int ino=getino(pathname);
	if(ino==0){
		printf("does not exists\n");
		return;
	}
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	
	myst.st_dev = dev;
	myst.st_ino = ino;
	myst.st_mode = ip->i_mode;
	myst.st_uid = ip->i_uid;
	myst.st_gid = ip->i_gid;
	myst.st_size = ip->i_size;
	myst.st_blksize = BLKSIZE;
	myst.st_atime = ip->i_atime;
	myst.st_ctime = ip->i_ctime;
	myst.st_mtime = ip->i_mtime;
	myst.st_nlink = ip->i_links_count;
	myst.st_blocks = ip->i_blocks;

	printf("dev: %d\n", myst.st_dev);
	printf("ino: %d\n", myst.st_ino);
	printf("mode: %x\n", myst.st_mode);
	printf("uid: %d\n", myst.st_uid);
	printf("gid: %d\n", myst.st_gid);
	printf("size: %d\n", myst.st_size);
	printf("links count: %d\n", myst.st_nlink);
	printf("atime: %s", ctime(&myst.st_atime));
	printf("mtime: %s", ctime(&myst.st_mtime));
	printf("ctime: %s", ctime(&myst.st_ctime));

	iput(mip);	
}

void mychmod(){
	if(!strcmp(pathname, "")) return;
	if(!strcmp(dest, "")) return;
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;
	int ino=getino(pathname);
	if(ino==0){
		printf("no such file\n");
		return;
	}
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	
	int n=0;
	char *slot[64];
	char *temp;

	temp=strtok(dest, "|");
	while(temp!=NULL){
		slot[n]=temp;
		n++;
		temp=strtok(0, "/");
	}
	int m=0;
	int modes[64];
	for(int i=0; i<n; i++){
		if(slot[i][0]=='0'){
			if(slot[i][1]=='x') modes[m]=strtol(slot[i], 0, 16);
			else modes[m]=strtol(slot[i], 0, 8);
		}
		else if(atoi(slot[i])) modes[m]=atoi(slot[i]);
		else continue;
		m++;
	}


	for(int i=0; i<m; i++){
		ip->i_mode=modes[i];
	}
	mip->dirty=1;
	iput(mip);
}

void touch(){
	if(!strcmp(pathname, "")) return;
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;
	int ino=getino(pathname);
	if(ino==0){
		printf("does not exists, creating the file\n");
		creat_file();
		return;
	}
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	
	ip->i_atime=time(0L);
	ip->i_mtime=time(0L);
	mip->dirty=1;
	
	iput(mip);
}











