/*
 * Andrea Kimball
 * FAT Part 1
 * gcc -Wall my_fat.c `pkg-config fuse --cflags --libs` -o my_fat
*/

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdbool.h>




/*
it's all one big array

index zero - superblock
indez 10000 - FAT
rest - root dir

array indices contain the number of the block with the next info in mem



need a get free block function
this checks a pointer in the superblock to the next free block
this is then moved to what that one says is the next free block
   check that this block is still free (has the next index)





*/




// block
typedef struct{
   int size;
   int value; // 0 if end, next number if free, otherwise next block that contains data

} block;




/*

magic number to identify fat - use BDAY
size of directory
block address of FAT
block address of root dir
block that is the next free block

superblock struct - 6 fields
*/


typedef struct{
   int magic;
   struct block *freelist; // pointer to free list


} fat_superblock;



//directory entry
typedef struct{
   bool isFile; //directory or file
   int size;  //size in bytes 
   int blocknumber; // number of the first block
   char name[];  // file name
} directoryentry;



// to make it easy to store the superblock in a FS block
union {
    struct fat_superblock s;
    char pad[4096];
}
superblock;




//macro to seek to a particular block

//macro to convert between byte offsets and block numbers


static void* fat_init(struct fuse_conn_info *conn){


/*open superblock file
read superblock
check magic # - birthday
if any of this failed
create superblock and initializes everything
all indices are the next number (for the free list)
theres an open FUSE O_CREAT flag


setting it up in more detail:
open file
doesnt exist (zero length)
allocate superblock using malloc
FAT - for loop that sets it all up initializes free list
write everything

root directory - trickier :/



*/





}


static int fat_getattr(const char *path, struct stat *stbuf)
{
// essentially ls -l 	

int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int fat_access(const char *path, int mask)
{
	int res;

	res = access(path, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int fat_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	
// read out of directory

DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(path);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int fat_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;


   //allocate space  
   struct block *newBlock = malloc(block_size);

   // currently puts block in free list - change this
   LIST_INSERT_HEAD(&freelist, newBlock, pointer);

	return 0;
}



static struct fuse_operations fat_oper = {
   .init = fat_init,	
   .getattr	= fat_getattr,
	.access		= fat_access,
	.readdir	= fat_readdir,
	.mkdir		= fat_mkdir,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &fat_oper, NULL);
}
