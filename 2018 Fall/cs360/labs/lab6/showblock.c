#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ext2fs/ext2_fs.h>

#define blksize 1024
#define base 1024

// define shorter TYPES
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR; 

// define pointer variables
GD    *gp;
SUPER *sp;
INODE *ip;
DIR *dp;
char *cp, *disk, *path[64];

static unsigned int blk_size = 0;

int inode_count, block_count, inode_size, block_size, inode_bitmap, block_bitmap, inode_table, npath = 0;

void get_block(int fd, int blk, char *buf)
{
	lseek(fd, (long)(blk * blksize), 0);
	read(fd, buf, blksize);
}

static void get_inode(int fd, int ino, INODE *inode)
{
	lseek(fd, blksize*inode_table + (ino - 1) * sizeof(INODE), SEEK_SET);
	read(fd, inode, sizeof(INODE));
}

void tokenize(char *full_path)
{
	char *temp;
	temp = strtok(full_path, "/");
	while (temp != NULL)
	{
		path[npath] = temp;
		npath++;
		temp = strtok(0, "/");
	}
}

int main(int argc, char *argv[])
{ 
	int fd, ino = 2, i;
	char full_path[blksize], backup[blksize];
	if (argc > 2)
	{
		printf("\n");
		disk = argv[1];
		strcpy(full_path, argv[2]);
	}
	else
	{
		printf("correct usage: ./a.out disk path");
		exit(0);
	}

	fd = open(disk, O_RDONLY);
	if (fd < 0)
	{
		printf("open failed %s\n", disk);
		exit(0);
	}

	strcpy(backup, full_path);
	if (strcmp("/", backup)) tokenize(full_path);
	printf("n=%d /", npath);
	for (i = 0; i < npath; i++) printf("%s/", path[i]);
	printf("\n");
	super(fd);
	getchar();
	get_values(fd);
	getchar();
	print_root(fd);
	getchar();
	if (strcmp("/", backup)) ino = search(fd);
	print_info(fd, ino, backup);
	close(fd);
	return 0;
}

void super(int fd)
{
	char buf[blksize];
	get_block(fd, 1, buf);
	sp = (SUPER *)buf;

	if (sp->s_magic != 0xEF53 && sp->s_magic != 0xEF51)
	{
		printf("not a valid disk\n");
		exit(0);
	}

	inode_count = sp->s_inodes_count;
	block_count = sp->s_blocks_count;
	inode_size = sp->s_inode_size;
	block_size = sp->s_log_block_size;
	blk_size = 1024 << block_size;

	printf("\nsuperblock\n");
	printf("inodes_count = %d\n", inode_count);
	printf("blocks_count = %d\n", block_count);
	printf("r_blocks_count = %d\n", sp->s_r_blocks_count);
	printf("free_blocks_count = %d\n", sp->s_free_blocks_count);
	printf("free_inodes_count = %d\n", sp->s_free_inodes_count);
	printf("log_block_size  = %d\n", sp->s_log_block_size);
	printf("first_data_block = %d\n", sp->s_first_data_block);
	printf("magic = %x\n", sp->s_magic);
	printf("blocks_per_group = %d\n", sp->s_blocks_per_group);
	printf("inodes_per_group = %d\n", sp->s_inodes_per_group);
	printf("rev_level = %d\n", sp->s_rev_level);
	printf("first_ino = %d\n", sp->s_first_ino);
	printf("inode_size = %d\n", inode_size);

	return;
}

void get_values(int fd)
{
	char buf[blksize];

	get_block(fd, 2, buf);
	gp = (GD *)buf;
	block_bitmap = gp->bg_block_bitmap;
	inode_bitmap = gp->bg_inode_bitmap;
	inode_table = gp->bg_inode_table;

	printf("\ngp info\n");
	printf("block_bitmap = %d\n", gp->bg_block_bitmap);
	printf("inode_bitmap = %d\n", gp->bg_inode_bitmap);
	printf("inode_table = %d\n", gp->bg_inode_table);
	printf("free_blocks_count = %d\n", gp->bg_free_blocks_count);
	printf("free_inodes_count = %d\n", gp->bg_free_inodes_count);
	printf("dirs_count = %d\n", gp->bg_used_dirs_count);
	printf("inode_start = %d", gp->bg_inode_table);
	
	printf("\n");
	ip = (INODE *)malloc(inode_size);
	lseek(fd, 1024 * inode_table + inode_size, SEEK_SET);
	read(fd, ip, inode_size);
	printf("\nroot inode\n");
	printf("mode = %x\n", ip->i_mode);
	printf("size = %d\n", ip->i_size);
	printf("n blocks=%d\n", ip->i_blocks);
	printf("block[0]=%d\n", ip->i_block[0]);
	printf("uid=%d\n", ip->i_uid);
	printf("gid=%d\n", ip->i_gid);
}

void print_root(int fd)
{
	INODE dirnode;
	get_inode(fd, 2, &dirnode);
	char buf[blksize], temp[256];

	printf("\nroot directory\n");
	for (int i = 0; i < 12; i++)
	{
		if (dirnode.i_block[i] == 0) return;
		get_block(fd, dirnode.i_block[i], buf);
		dp = (DIR *)buf;
		cp = buf;

		while (cp < buf+blksize)
		{
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len] = 0; //end with null terminal
			printf("i_n: %d\t rec_len: %d\t name_len: %d\t name: %s\n", dp->inode, dp->rec_len, dp->name_len, temp);

			cp += dp->rec_len;
			dp = (DIR *)cp;
		}
	}
}

int search(int fd)
{
	int inumber=0;
	INODE dirnode;
	get_inode(fd, 2, &dirnode);
	for (int i = 0; i < npath; i++)
	{
		if (!S_ISDIR(dirnode.i_mode)){
			printf("is not a directory\n");
			exit(0);
		}
		inumber = sub_search(fd, dirnode, path[i]);
		if (inumber == 0)
		{
			printf("not exist\n");
			exit(0);
		}
		printf("found %s: inode=%d\n", path[i], inumber);
		get_inode(fd, inumber, &dirnode);
	}
	return inumber;
}

int sub_search(int fd, INODE dirnode, char *filename)
{
	char buf[blksize], temp[256];
	printf("\n%s\n", filename);
	for (int i = 0; i < 12; i++)
	{
		if (dirnode.i_block[i] == 0) return 0;
		get_block(fd, dirnode.i_block[i], buf);
		dp = (DIR *)buf;
		cp = buf;

		while (cp < blksize+buf)
		{
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len] = 0;
			printf("n_i: %d\t rec_len: %d\t name_len: %d\t name: %s\n", dp->inode, dp->rec_len, dp->name_len, temp);

			if (!strcmp(filename, temp)) return dp->inode;
			cp += dp->rec_len;
			dp = (DIR *)cp;
		}
	}
	return 0;
}

void print_info(int fd, int ino, char *name)
{
	int indirect[256], double_indirect[256];	
	INODE file;
	SUPER super;

	lseek(fd, base, SEEK_SET); 
	read(fd, &super, sizeof(super));
	blk_size = 1024 << super.s_log_block_size;
	get_inode(fd, ino, &file);
	int num_blocks = file.i_size / blksize+1;
	printf("\nsize: %u blocks: %u\n", file.i_size, num_blocks);

	printf("\n====DISK BLOCKS====\n");
	for (int i = 0; i < 14; i++) printf("block[%d]: %d\n", i, file.i_block[i]);

	printf("\n====DIRECT BLOCKS====\n");
	int cycle_blocks = num_blocks;
	if (cycle_blocks > 12) cycle_blocks = 12;
	for(int i=0; i<cycle_blocks; i++) printf("%d ", file.i_block[i]);
	num_blocks -= cycle_blocks;
	printf("\nblocks remain: %u\n", num_blocks);

	if (num_blocks > 0){
		printf("====indirect blocks====\n");
		cycle_blocks = num_blocks;
		if (cycle_blocks > 256) cycle_blocks = 256;
		get_block(fd, file.i_block[12], indirect);
		for(int i=0; i<cycle_blocks; i++) printf("%d ", indirect[i]);
		num_blocks -= cycle_blocks;
		printf("\nblocks Remain: %u\n", num_blocks);
	}
	if (num_blocks > 0){
		printf("====double indirect blocks====\n");
		get_block(fd, file.i_block[13], double_indirect);
		for (int i = 0; i < 256; i++){
			if (double_indirect[i] == 0) break;
			printf("%d\n----\n", double_indirect[i]);
			cycle_blocks = num_blocks;
			if (cycle_blocks > 256) cycle_blocks = 256;
			get_block(fd, double_indirect[i], indirect);
			for(int i=0; i<cycle_blocks; i++) printf("%d ", indirect[i]);
			num_blocks -= cycle_blocks;
			printf("\nblocks remain: %u\n", num_blocks);
		}
	}
}
