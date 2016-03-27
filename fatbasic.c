
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

#define BLOCK_SIZE 4069
#define N 100000





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



void buildFat(){
      printf("%s\n", "hello");

struct block superBlock;


struct block *list;
list = (struct block *) malloc (N);



superBlock.magic = 0515;
superBlock.directorysize = 2;
superBlock.FAT = 10000;
superBlock.root = 10001;
superBlock.free = 1;
superBlock.value = 0;

list[0] = superBlock;

int i;
int ct =0;
for (i = 1; i < N; i++){
   ct++;   
   printf("%d\n", ct);
   struct block temp;
   temp.magic = 0;
   temp.directorysize = 0;
   temp.FAT = 0;
   temp.root = 0;
   temp.free = 0;
   temp.value = i+1;
   
   list[i] = temp;
}

for(i = 0; i < 3; i++) {
    printf("%d \n", list[i].value);
}


return;

}


int main(int argc, char *argv[])
{

FILE *fatsystem;
char *FAT;
int size;

fatsystem = fopen(FAT, "ab+");
fseek(fatsystem, 0L, SEEK_END);
size = ftell(fatsystem);
fseek(fatsystem, 0L, SEEK_SET);

printf("%d\n", size);

   if (size==0){

      buildFat();



   } else{


      
      
   }




	return 0;
}
