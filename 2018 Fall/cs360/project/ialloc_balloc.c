int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  if (buf[i] & (1 << j))
     return 1;
  return 0;
}

int set_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  buf[i] |= (1 << j);
}

int clr_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  buf[i] &= ~(1 << j);
}

int decFreeBlocks(int dev){
	char buf[BLKSIZE];
	get_block(dev, 1, buf);
	sp=(SUPER*)buf;
	sp->s_free_blocks_count--;
	put_block(dev, 1, buf);

	get_block(dev, 2, buf);
	gp=(GD*)buf;
	gp->bg_free_blocks_count--;
	put_block(dev, 2, buf);
}

int decFreeInodes(int dev)
{
  char buf[BLKSIZE];

  // dec free inodes count in SUPER and GD
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;
  sp->s_free_inodes_count--;
  put_block(dev, 1, buf);

  get_block(dev, 2, buf);
  gp = (GD *)buf;
  gp->bg_free_inodes_count--;
  put_block(dev, 2, buf);
}

int ialloc(int dev)
{
  int  i;
  char buf[BLKSIZE];

  // read inode_bitmap block
  get_block(dev, imap, buf);

  for (i=0; i < ninodes; i++){
    if (tst_bit(buf, i)==0){
       set_bit(buf,i);
       decFreeInodes(dev);

       put_block(dev, imap, buf);

       return i+1;
    }
  }
  printf("ialloc(): no more free inodes\n");
  return 0;
}

int balloc(int dev){
	char buf[BLKSIZE];

	get_block(dev, bmap, buf);

	for(int i=0; i<nblocks; i++){
		if(tst_bit(buf, i)==0){
			set_bit(buf, i);
			decFreeBlocks(dev);
			
			put_block(dev, bmap, buf);
			return i+1;
		}
	}
	return 0;
}

int idealloc(int dev, int ino){
	char buf[BLKSIZE];
	
	get_block(dev, imap, buf);
	clr_bit(buf, ino-1);
	put_block(dev, imap, buf);
	get_block(dev, 1, buf);
	sp=(SUPER*)buf;
	sp->s_free_inodes_count++;
	put_block(dev, 1, buf);
	get_block(dev, 2, buf);
	gp=(GD*)buf;
	gp->bg_free_inodes_count++;
	put_block(dev, 2, buf);
}

int bdealloc(int dev, int bno){
	char buf[BLKSIZE];
	get_block(dev, bmap, buf);
	clr_bit(buf, bno-1);
	put_block(dev, bmap, buf);
	get_block(dev, 1, buf);
	sp=(SUPER*)buf;
	sp->s_free_blocks_count++;
	put_block(dev, 1, buf);
	get_block(dev, 2, buf);
	gp=(GD*)buf;
	gp->bg_free_blocks_count++;
	put_block(dev, 2, buf);
}

int truncate(INODE *ip){
	char buf[BLKSIZE], tbuf[BLKSIZE];
	for(int i=0; i<12; i++){
		if(ip->i_block[i]==0) break;
		bdealloc(dev, ip->i_block[i]);
	}
	if(ip->i_block[12]==0) return 1;
	get_block(dev, ip->i_block[12], buf);
	for(int i=0; i<256; i++){
		if(buf[i]==0) break;
		bdealloc(dev, buf[i]);
	}
	bdealloc(dev, ip->i_block[12]);
	if(ip->i_block[13]==0) return 1;
	memset(buf, 0, 256);
	get_block(dev, ip->i_block[13], buf);
	for(int i=0; i<256; i++){
		if(buf[i]==0) break;
		get_block(dev, buf[i], tbuf);
		for(int j=0; j<256; j++){
			if(tbuf[j]==0) break;
			bdealloc(dev, tbuf[j]);
		}
		bdealloc(dev, buf[i]);
	}
	bdealloc(dev, ip->i_block[13]);
	ip->i_atime=time(0L);
	ip->i_mtime=time(0L);
	ip->i_size=0;
	return 1;
}






