#include "main.h"

int main(){
  

// Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_WRITER_OFLAG, MY_SHM_OPEN_WRITER_MODE);

    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, MY_DATA_SIZE);

    // Get a pointer to the data
    Share*sharedData = mmap(NULL, MY_DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for(int i=0;i<10;i++){
        sharedData->number[i]= i+1;//(rand() % 5) + 1; ;
        strcpy(sharedData->stringIsep[i], "ISEP – SCOMP 2020");
        //printf("\nPosicao:%d -> %s\n",i,sharedData->stringIsep[i]);
    }

    // Undo mapping
    if (munmap((void *)sharedData, MY_DATA_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }


    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
 
return 0;
}
