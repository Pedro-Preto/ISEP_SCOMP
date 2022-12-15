#include "main.h"

int main() {

    // Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_READER_OFLAG, MY_SHM_OPEN_READER_MODE);

    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Defines the size
    ftruncate(fd, MY_DATA_SIZE);

    // Get a pointer to the data
  Share *sharedData = (Share *)mmap(NULL, MY_DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


printf("\n===========================================\n");

    // Print the data
    int average=0;
    for(int i=0;i<10;i++){
    printf("Dados: %d\n", sharedData->number[i]);
    average+=sharedData->number[i];
    }
    average/=10;

    printf("Media: %d\n",average);

printf("===========================================\n");

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

    // Remove file from system
    if (shm_unlink(MY_SHARED_FILE) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}