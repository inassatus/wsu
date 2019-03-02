int mount(){
	int show=0;
	if(!strcmp(pathname, "")) show=1;
	if(!strcmp(dest, "")) show=1;
	if(show){
		for(int i=0; i<NMOUNT; i++){
			printf("name: %s\tdev: %d\n", mounts[i][0]->name, mounts[i][0]->dev);
		}
		return;
	}
	char path[256], point[256];
	strcpy(path, pathname);
	strcpy(point, dest);
	if(pathname[0]=='/') dev=root->dev;
	else dev=running->cwd->dev;
	int ino=getino(pathname);
	MINODE *mip=iget(dev, ino);
	if(mip->mounted){
		printf("already mounted\n");
		iput(mip);
		return;
	}
	int i;
	for(i=0; i<NMOUNT; i++){
		if(mounts[i]==0){
			mounts[i]=&mip->mptr;
			mip->mounted=1;
		}
		if(i==NMOUNT-1){
			printf("already mounted too many\n");
			iput(mip);
			return;
		}
	}
	strcpy(pathname, path);
	strcpy(dest, "2");
	mount[i]->dev=open_file();
	if(mount[i]->dev<0){
		printf("error occurred, could not open\n");
		iput(mip);
		return;
	}	
	
}
