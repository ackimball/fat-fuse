
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


int main(int argc, char *argv[])
{

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
