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
#include <sys/file.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define N 20480
#define PERMS 0666






struct directoryEntry {
   bool isFile; //directory or file
   int size;  //size in bytes 
   int blocknumber; // number of the first block
   char name[];  // file name
};

struct block {
   int magic; // for ID
   int directorysize;
   int FAT; // address of FAT
   int root; // address of root directory
   int free; // next free block

   int value; // not used in superblock
};

struct block getFree(struct block superB, struct block *list){
   int freeIndex = superB.free;
   struct block freeBlock = list[freeIndex];
   freeBlock.free = 0;

   int nextFreeIndex = freeBlock.value;
   struct block nextFreeBlock = list[nextFreeIndex];
   
   if (nextFreeBlock.free == 15){
      superB.free = nextFreeIndex;
   } else {
      while (nextFreeBlock.free != 15) {
         nextFreeIndex++;
         nextFreeBlock = list[nextFreeIndex];
      }
      superB.free = nextFreeIndex;
   } 
   return freeBlock;
}



struct block* buildFat(){
   struct block superBlock;
   struct block *list;
   list = (struct block *) malloc (N * BLOCK_SIZE);

   superBlock.magic = 0515;
   superBlock.directorysize = 2;
   superBlock.FAT = 10000;
   superBlock.root = 10001;
   superBlock.free = 1;
   superBlock.value = 0;

   list[0] = superBlock;

   int i;
   int ct = 0;
   for (i = 1; i <= N; i++){
      ct++;   
      printf("%d\n", ct);
      struct block temp;
      temp.magic = 0;
      temp.directorysize = 0;
      temp.FAT = 0;
      temp.root = 0;
      temp.free = 15;
      temp.value = i+1;

      list[i] = temp;
   }

   for(i = 0; i < 5; i++) {
      printf("%d \n", list[i].value);
   }
   
   return list;
}




static void* fat_init(struct fuse_conn_info *conn){

   char *FAT = "FAT";
   int fd;
   fd = open(FAT, O_RDWR, PERMS);


   if (fd == -1){
      fd = creat(FAT, PERMS);

      if (fd != -1){
         printf("%s\n", "created file");
      } 
      
      struct block *list;
      list = buildFat();

      char *b;
      b = (char *) list;

      write(fd, b, N);
      printf("%s\n", "wrote");
      } 

     close(fd);
	
	return 0;


}

static int fat_getattr(const char *path, struct stat *stbuf){
// essentially ls -l 	
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

	return 0;
}

static int fat_chmod(const char *path, mode_t mode){
   return 0;
}

static int fat_open(const char *path, struct fuse_file_info *fi){
	return 0;
}

static int fat_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	return 0;
}

static int fat_write(const char* path, const char *buf, size_t size, off_t offset,  struct fuse_file_info *fi){
	return 0;
}

static int fat_truncate(const char *path, off_t size){
	return 0;
}

static int fat_mknod(const char *path, mode_t mode, dev_t dev){
	return 0;
}

static int fat_create(const char *path, mode_t mode, struct fuse_file_info *fi){
	return 0;
}

static int fat_chown(const char *path, uid_t uid, gid_t gid){
	return 0;
}	


static struct fuse_operations fat_oper = {
   	.init 		= fat_init,	
   	.getattr	= fat_getattr,
	.access		= fat_access,
	.readdir	= fat_readdir,
	.mkdir		= fat_mkdir,
	.chmod		= fat_chmod,
	.open		= fat_open,
	.read		= fat_read,
	.write		= fat_write,
	.truncate	= fat_truncate,
	.mknod		= fat_mknod,
	.create		= fat_create,
	.chown		= fat_chown,

};



int main(int argc, char *argv[]){
	return fuse_main(argc, argv, &fat_oper, NULL);
}





