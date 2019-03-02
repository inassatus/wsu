int mywrite(int fd, char *buf, int nbytes){
	OFT *oftp = running->fd[fd];
	int lbk;
	int blk;
	int startbyte;
	char tbuf[BLKSIZE];
	char wbuf[BLKSIZE];
	MINODE *mip=oftp->mptr;
	int remain;
	char cp=0;
	int get;
	while(nbytes>0){
		lbk=oftp->offset/BLKSIZE;
		startbyte=oftp->offset%BLKSIZE;
		if(lbk<12){
			if(mip->INODE.i_block[lbk]==0) mip->INODE.i_block[lbk]=balloc(mip->dev);
			blk=mip->INODE.i_block[lbk];	
		}
		else if(lbk>=12&&lbk<256+12){
			if(mip->INODE.i_block[12]==0){
				mip->INODE.i_block[12]=balloc(mip->dev);
				get_block(mip->dev, mip->INODE.i_block[12], tbuf);
				*tbuf=0;
				put_block(mip->dev, mip->INODE.i_block[12], tbuf);
			}
			get_block(mip->dev, mip->INODE.i_block[12], tbuf);
			blk=tbuf+lbk-12;
			if(blk==0){
				tbuf[lbk-12]=balloc(mip->dev);
				blk=tbuf[lbk-12];
				put_block(mip->dev, mip->INODE.i_block[12], tbuf);
			}
		}
		else{
			if(mip->INODE.i_block[13]==0){
				mip->INODE.i_block[13]=balloc(mip->dev);
				get_block(mip->dev, mip->INODE.i_block[13], tbuf);
				*tbuf=0;
				put_block(mip->dev, mip->INODE.i_block[13], tbuf);
			}
			get_block(mip->dev, mip->INODE.i_block[13], tbuf);
			int iblk=(lbk-256-12)/256;
			int ioff=(lbk-256-12)%256;
			blk=tbuf+iblk;
			if(blk==0){
				tbuf[iblk]=balloc(mip->dev);
				blk=tbuf[iblk];
				get_block(mip->dev, blk, tbuf);
				*tbuf=0;
				put_block(mip->dev, blk, tbuf);
			}
			get_block(mip->dev, blk, tbuf);
			blk=tbuf+ioff;
			if(blk==0){
				tbuf[ioff]=balloc(mip->dev);
				blk=tbuf[ioff];
				put_block(mip->dev, blk, tbuf);
			}
		}
		get_block(mip->dev, blk, wbuf);
		char *cq=wbuf+startbyte;
		remain=BLKSIZE-startbyte;
		get=nbytes<remain?nbytes:remain;
		nbytes-=get;
		oftp->offset+=get;
		mip->INODE.i_size=(int*)oftp->offset;
		strncpy(cq, buf+cp, cp+get);
		cp+=get;
		/*give algo		
		while(remain>0){
			*cp++=*cq++;
			nbytes--;
			remain--;
			oftp->offset++;
			if(oftp->offset>mip->INODE.i_size) mip->INODE.i_size+=1;
			if(nbytes<=0) break;
		}*/
		put_block(mip->dev, blk, wbuf);
	}
	mip->dirty=1;
	return nbytes;
}

int write_file(){
	if(!strcmp(pathname, "")) return -1;
	if (!strcmp(dest, "")) return -1;
	int fd=atoi(pathname);
	if(running->fd[fd]==0){
		printf("is not opened\n");
		return -1;
	}
	int byte=strlen(dest);
	char buf[256];
	strcpy(buf, dest);
	if(running->fd[fd]->mode==1||running->fd[fd]->mode==2||running->fd[fd]->mode==3) return (mywrite(fd, buf, byte));
	else{
		printf("write failed\n");
		return -1;
	}
}

int mycp(){
	if(!strcmp(pathname, "")) return -1;
	if (!strcmp(dest, "")) return -1;
	char temp1[256], temp2[256];
	strcpy(temp1, pathname);
	strcpy(temp2, dest);

	strcpy(dest, "0");
	int fd=open_file();
	if(fd<0) return -1;
	strcpy(pathname, temp2);
	strcpy(dest, "1");
	int gd=open_file();
	if(gd<0){
		close_file(fd);
		return -1;
	}
	int n;
	char buf[4096];
	while(n=myread(fd, buf, 4096)){
		mywrite(gd, buf, n);
	}
	close_file(fd);
	close_file(gd);
}

int mymv(){
	if(!strcmp(pathname, "")) return 0;
	if (!strcmp(dest, "")) return 0;
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;

	char temp1[256], temp2[256];
	char temp[256];
	strcpy(temp1, pathname);
	strcpy(temp2, dest);
	strcpy(temp, pathname);
	char base[256];
	strcpy(base, basename(temp));
	strcat(dest, "/");
	strcat(dest, base);
	int linked=link();
	if(!linked) return 0;
	strcpy(pathname, temp1);
	ulink();
	return 1;
}

