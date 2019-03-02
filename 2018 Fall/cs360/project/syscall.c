int open_file(){
	char path[256];
	strcpy(path, pathname);
	if(!strcmp(pathname, "")) return -1;
	if(!strcmp(dest, "")) return -1;
	int mode;
	if(!strcmp(dest, "0")||!strcmp(dest, "R")||!strcmp(dest, "r")) mode=0;
	else if(!strcmp(dest, "1")||!strcmp(dest, "W")||!strcmp(dest, "w")) mode=1;
	else if(!strcmp(dest, "2")||!strcmp(dest, "RW")||!strcmp(dest, "rw")) mode=2;
	else if(!strcmp(dest, "3")||!strcmp(dest, "APPEND")||!strcmp(dest, "append")) mode=3;
	else{
		printf("not a valid mode\n");
		if(dest[0]=='|'||dest[1]=='|') printf("pipe is not included in the given design\n");
		return -1;
	}
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	int ino=getino(pathname);

	if(ino==0){
		if(mode==0||mode==3){
			printf("no such file exists\n");
			return -1;
		}
		strcpy(pathname, path);
		ino=creat_file();
	}
	if(ino==0){
		printf("failed to open\n");
	}

	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;

	if(!S_ISREG(ip->i_mode)){
		printf("not regular\n");
		iput(mip);
		return -1;
	}
	if(running->pid==1||running->uid==ip->i_uid||running->gid==ip->i_gid);
	else{
		printf("access denied\n");
		iput(mip);
		return -1;
	}

	for(int i=0; i<NOFT; i++){
		if(&oft[i]==0) break;
		if(oft[i].mptr==mip){
			if(mode!=0&&oft[i].mode==mode&&oft[i].refCount>0){
				printf("this file is already opened\n");
				iput(mip);
				return -1;
			}
		}
	}

	OFT *oftp;
	for(int i=0; i<NOFT; i++){
		if(&oft[i]==0||oft[i].refCount<1){
			oftp=&oft[i];
			break;
		} 
		if(i==NOFT-1){
			printf("too many files are opened\n");
			iput(mip);
			return -1;
		}
	}

	int i=0;
	for(i; i<NFD; i++){
		if(running->fd[i]==0) break;
		if(i==NFD-1){
			printf("fd limit, could not open\n");
			iput(mip);
			return -1;
		}
	}

	oftp->mode=mode;
	oftp->refCount=1;
	oftp->mptr=mip;
	switch(mode){
		case 0:
			oftp->offset=0;
			break;
		case 1:
			truncate(ip);
			mip->dirty=1;
			oftp->offset=0;
			break;
		case 2:
			oftp->offset=0;
			break;
		case 3:
			oftp->offset=ip->i_size;
			break;
		default:
			printf("not a vaild mode\n");
			iput(mip);
			return -1;
	}
	running->fd[i]=oftp;
	strcpy(pathname, path);
	if(mode==0) touch();
	else{
		ip->i_atime=time(0L);
		ip->i_mtime=time(0L);
		mip->dirty=1;
	}
	return i;
}

void close_file(int fd){
	if(fd<0||fd>NFD-1){
		printf("invalid fd\n");
		return;
	}
	if(running->fd[fd]==0){
		printf("file is not opened\n");
		return;
	}
	OFT *oftp=running->fd[fd];
	running->fd[fd]=0;
	oftp->refCount--;
	if(oftp->refCount>0) return;
	MINODE *mip=oftp->mptr;
	iput(mip);
}

int mylseek(int fd, int position){
	if(running->fd[fd]==0){
		printf("file is not opened\n");
		return -1;
	}
	if(position<running->fd[fd]->mptr->INODE.i_size+1&&position>-1) running->fd[fd]->offset=position;
	else{
		printf("over run\n");
		return -1;
	}
	return position;
}

void pfd(){
	for(int i=0; i<NFD; i++){
		if(running->fd[i]==0) return;
		printf("fd: %d\tmode: %d\toffset: %d\tINODE: [%d, %d]\n", i, running->fd[i]->mode, running->fd[i]->offset, running->fd[i]->mptr->dev, running->fd[i]->mptr->ino);
	}
}
