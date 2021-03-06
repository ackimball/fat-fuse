/*
 * Andrea Kimball
 * FAT Part 1
gcc -Wall fat-fuse.c `pkg-config fuse --cflags --libs` -o fat-fuse
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


typedef struct{
   int magic; // for ID
   int directorysize;
   int FAT; // address of FAT
   int root; // address of root directory
   int free; // next free block

   int value; // not used in superblock
} block;



//directory entry
typedef struct{
   bool isFile; //directory or file
   int size;  //size in bytes 
   int blocknumber; // number of the first block
   char name[];  // file name
} directoryentry;



// to make it easy to store the superblock in a FS block
/*union {
    struct superblock s;
    char pad[4096];
} store_superblock;
*/


//macro to return next free block
/*this checks a pointer in the superblock to the next free block
this is then moved to what that one says is the next free block
   check that this block is still free (has the next index) */

//macro to seek to a particular block

//macro to convert between byte offsets and block numbers


static void* fat_init(/*struct fuse_conn_info *conn*/){


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




// if we're setting it up
block superBlock;
block array[10000*1000];


superBlock.magic = 0515;
superBlock.directorysize = 2;
superBlock.FAT = 10000;
superBlock.root = 10001;
superBlock.free = 1;
superBlock.value = 0;

array[0] = superBlock;

int i;
for (i = 1; i < 10000*1000; i++){
   block temp;
   temp.magic = 0;
   temp.directorysize = 0;
   temp.FAT = 0;
   temp.root = 0;
   temp.free = 0;
   temp.value = i+1;
   
   array[i] = temp;
}

for(i = 0; i < 3; i++) {
    printf("%d ", array[i].value);
}


/*
index zero - superblock
indez 10000 - FAT
rest - root dir

array indices contain the number of the block with the next info in mem
*/

return 0;



}


/*static int fat_getattr(const char *path, struct stat *stbuf)
{
// essentially ls -l 	
return 0;
}*/




/*static struct fuse_operations fat_oper = {
   .init = fat_init,	
   .getattr	= fat_getattr,
	.access		= fat_access,
	.readdir	= fat_readdir,
	.mkdir		= fat_mkdir,
};*/

int main(int argc, char *argv[])
{
	return 0;
}
