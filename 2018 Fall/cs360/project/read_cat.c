int read_file(){
	if(!strcmp(pathname, "")) return -1;
	if (!strcmp(dest, "")) return -1;
	int fd=atoi(pathname);
	int byte=atoi(dest);
	if(running->fd[fd]==0){
		printf("is not opened\n");
		return -1;
	}
	char buf[4096];
	if(running->fd[fd]->mode==0||running->fd[fd]->mode==2) return (myread(fd, buf, byte));
	else{
		printf("read failed\n");
		return -1;
	}
}

int myread(int fd, char *buf, int byte){
	int count=0;
	OFT *oftp = running->fd[fd];
	MINODE *mip=oftp->mptr;
	int avil = mip->INODE.i_size-oftp->offset;
	int lbk;
	int startbyte;
	int blk;
	int remain;
	char tbuf[256];
	char readbuf[BLKSIZE];
	int left=byte>avil?avil:byte;
	int get;
	int cp=0;
	while(left){
		lbk=oftp->offset/BLKSIZE;
		startbyte=oftp->offset%BLKSIZE;
		if(lbk<12){
			blk=mip->INODE.i_block[lbk];
		}
		else if(lbk>=12&&lbk<256+12){
			get_block(mip->dev, mip->INODE.i_block[12], tbuf);
			blk=tbuf+lbk-12;
		}
		else{
			get_block(mip->dev, mip->INODE.i_block[13], tbuf);
			int di = tbuf+(lbk-(256+12))/256;
			get_block(mip->dev, di, tbuf);
			blk=tbuf+(lbk-(256+12))%256;
		}
		get_block(mip->dev, blk, readbuf);
		char *cq=readbuf+startbyte;
		remain=BLKSIZE-startbyte;
		get=remain<left?remain:left;
		left-=get;
		count+=get;
		oftp->offset+=get;
		strncat(buf, cq+cp, cp+get);
		cp+=get;
		/* given algo
		while(remain>0){
			*cq++=*cp++;
			oftp->offset++;
			count++;
			avil--;
			byte--;
			remain--;
			if(byte<=0||avil<=0) break;
		}*/
	}
	return count;
}

void cat(){
	if(!strcmp(pathname, "")) return;
	char mybuf[4096];
	char dummy=0;
	int n;
	strcpy(dest, "0");
	int fd=open_file();
	if(fd<0) return;
	while(n=myread(fd, mybuf, 4096)){
		mybuf[n]=0;
		int i=0;
		while(mybuf[i]){
			putchar(mybuf[i]);
			i++;
		}
	}
	printf("\n");
	close_file(fd);
}
