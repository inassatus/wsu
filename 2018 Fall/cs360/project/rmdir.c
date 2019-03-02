void rm_child(MINODE *pmip, char *name){
	INODE *pip=&pmip->INODE;
	int removed, prev;
	DIR *dp;
	char buf[BLKSIZE];
	char temp[256];
	char *cp;
	int isfirst;

	for(int i=0; i<12; i++){
		if(pip->i_block[i]==0) return;
		get_block(dev, pip->i_block[i], buf);
		dp=(DIR*)buf;
		cp=buf;
		isfirst=1;
		while(cp<buf+BLKSIZE){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len]=0;
			if(!strcmp(temp, name)){
				if(isfirst){
					bdealloc(dev, pip->i_block[i]);
					pip->i_size-=BLKSIZE;
					for(i; i<11; i++){
						if(pip->i_block[i+1]==0) break;
						pip->i_block[i]=pip->i_block[i+1];
						put_block(dev, pip->i_block[i], buf);	
					}
					pip->i_block[i]=0;
					put_block(dev, pip->i_block[i], buf);
					pmip->dirty=1;
					return;
				}
				if(cp+dp->rec_len==buf+BLKSIZE){
					removed=dp->rec_len;
					cp-=prev;
					dp=(DIR*)cp;
					dp->rec_len+=removed;
					put_block(dev, pmip->INODE.i_block[i], buf);
					pmip->dirty=1;
					return;
				}
				removed=dp->rec_len;
				char *nextc=cp+dp->rec_len;
				DIR *nextd=(DIR*)nextc;
				while(cp+dp->rec_len+nextd->rec_len<buf+BLKSIZE){
					cp+=dp->rec_len;
					nextc+=nextd->rec_len;
					dp->inode=nextd->inode;
					dp->rec_len=nextd->rec_len;
					dp->name_len=nextd->name_len;
					strncpy(dp->name, nextd->name, nextd->name_len);
					dp=(DIR*)cp;
					nextd=(DIR*)nextc;
				}
				dp->inode=nextd->inode;
				dp->rec_len=nextd->rec_len+removed;
				dp->name_len=nextd->name_len;
				strncpy(dp->name, nextd->name, nextd->name_len);
				put_block(dev, pip->i_block[i], buf);
				pmip->dirty=1;
				return;
			}
			isfirst=0;
			prev=dp->rec_len;
			cp+=dp->rec_len;
			dp=(DIR*)cp;
		}
	}
}

int isempty(MINODE *mip)
{
	char buf[1024];
	INODE *ip=&mip->INODE;
	char *cp;
	char temp[64];
	DIR *dp;

	if(ip->i_links_count > 2) return 1;
	else if(ip->i_links_count==2){
		if(ip->i_block[0]==0) return 0;
		get_block(dev, ip->i_block[0], buf);

		cp=buf;
		dp=(DIR*)buf;

		while(cp < buf + 1024){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len]=0;

			if(strcmp(temp, ".")){
				if(strcmp(temp, "..")) return 1;
			}
			cp+=dp->rec_len;
			dp=(DIR*)cp;
		}
	}
	return 0;
}

void rmdir(){
	if(!strcmp(pathname, "")) return;
	if(!strcmp(pathname, "/")){
		printf("could not remove: currently opened\n");
		return;
	}
	char temp1[256], temp2[256];
	char dir[256], base[256];
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	strcpy(temp1, pathname);
	strcpy(temp2, pathname);
	strcpy(dir, dirname(temp1));
	strcpy(base, basename(temp2));

	int ino=getino(pathname);
	if(ino==0){
		printf("no such file exist\n");
		return;
	}
	MINODE *mip=iget(dev, ino);
	INODE *ip=&mip->INODE;
	if(running->pid!=1){
		if(running->uid!=ip->i_uid){
			printf("you are not able to rmdir\n");
			iput(mip);
			return;
		}
	}
	if(!S_ISDIR(ip->i_mode)){
		printf("not a directory\n");
		iput(mip);
		return;
	}
	if(running->cwd==mip){
		printf("is busy\n");
		iput(mip);
		return;
	}
	if(isempty(mip)){
		printf("is not empty\n");
		iput(mip);
		return;
	}
	int pino=getino(dir);
	MINODE *pmip=iget(dev, pino);
	INODE *pip=&pmip->INODE;
	
	truncate(ip);
	idealloc(dev, ino);
	rm_child(pmip, base);
	pip->i_links_count--;
	pip->i_atime=time(0L);
	pip->i_mtime=time(0L);
	mip->dirty=1;
	pmip->dirty=1;	
	iput(mip);
	iput(pmip);
}




