#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


typedef struct {
    int num1;
    int num2;
} shared_data_type;

int main(){
//data to be shared
    shared_data_type *shared_data;

    //size of data
    int size = sizeof(shared_data);

    // Creates and opens a shared memory area
    int fd = shm_open("/ex5", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    shared_data->num1=8000;
    shared_data->num2=200;


 // Undo mapping
    if (munmap(shared_data, size) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }



if(fork()>1){
int lim1=1;
    while(lim1 < 1000000){
     //data to be shared
    shared_data_type *shared_data1;

    //size of data

    // Creates and opens a shared memory area
     fd = shm_open("/ex5", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data1 = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    shared_data1->num1+=1;
    shared_data1->num2-=1;

    // Undo mapping
    if (munmap(shared_data1, size) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
    lim1++;
    } 
}else{
    
    int lim2=1;
    while(lim2 < 1000000){
     //data to be shared
    shared_data_type *shared_data2;

    

    // Creates and opens a shared memory area
     fd = shm_open("/ex5", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data2 = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    shared_data2->num1-=1;
    shared_data2->num2+=1;

    // Undo mapping
    if (munmap(shared_data2, size) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
    lim2++;
    }
    exit(EXIT_SUCCESS); 
}
//data to be shared
    shared_data_type *shared_data3;

    
    // Creates and opens a shared memory area
     fd = shm_open("/ex5", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data3 = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("Num1->%d\n",shared_data->num1);
    printf("Num2->%d\n",shared_data->num2);

 // Undo mapping
    if (munmap(shared_data3, size) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
}