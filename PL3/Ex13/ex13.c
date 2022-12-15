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
int spawnChilds(int n);
int countOccurrences(FILE *filePointer, const char *word);

#define MY_SHARED_FILE "/Ex_13"
#define NUMBER_OF_CHILDS 10
#define BUFFER_SIZE 1000

typedef struct {
    char filePath[NUMBER_OF_CHILDS][80];
    char wordToSearch[80];
    int numberOfOccurencies[NUMBER_OF_CHILDS];
} shared_data_type;

#define STRUCT_SIZE sizeof(shared_data_type)

int main(){
//==============================|
	//Criacao da memoria partilhada  	
    
	// Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
	// Defines the size
    ftruncate(fd, STRUCT_SIZE);

    // Get a pointer to the data
    shared_data_type *shared_data = (shared_data_type *)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
//==============================|
//Carregar os dados para a memoria partilhada

strcpy(shared_data->wordToSearch, "ISEP");

for(int i=0;i<NUMBER_OF_CHILDS;i++){

    char buffer[15];
    char ext[15]=".txt";
    snprintf(buffer, sizeof(buffer), "file%d", i+1);
    strcat(buffer, ext);
    strcpy(shared_data->filePath[i],buffer);
    //printf("%s\n",shared_data->filePath[i]);
    shared_data->numberOfOccurencies[i]=0;
}

//==============================|

int id=spawnChilds(NUMBER_OF_CHILDS);

    for(int i=1;i<NUMBER_OF_CHILDS+1;i++){
        if(id==i){

        FILE *filePointer;
        filePointer = fopen(shared_data->filePath[i-1],"r");

        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }
        
        shared_data->numberOfOccurencies[i-1]=countOccurrences(filePointer, shared_data->wordToSearch);
    
        exit(EXIT_SUCCESS);
        }
    }
//======================Parent
    for(int i=0;i<NUMBER_OF_CHILDS;i++){
        wait(NULL);
    }

	printf("======================================\n");


    for(int i=0;i<NUMBER_OF_CHILDS;i++){
        printf("Ficheiro %d -> Palavras Encontradas:%d\n",i+1,shared_data->numberOfOccurencies[i]);
    }

	printf("======================================\n");

    // Undo mapping
    if (munmap(shared_data, STRUCT_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
   if (shm_unlink(MY_SHARED_FILE) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
	}	
}

int spawnChilds(int n){
	
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int countOccurrences(FILE *filePointer, const char *word){
    char str[BUFFER_SIZE];
    char *pos;

    int index, count;
    
    count = 0;

    // Read line from file till end of file.
    while ((fgets(str, BUFFER_SIZE, filePointer)) != NULL)
    {
        index = 0;

        // Find next occurrence of word in str
        while ((pos = strstr(str + index, word)) != NULL)
        {
            // Index of word in str is
            // Memory address of pos - memory
            // address of str.
            index = (pos - str) + 1;

            count++;
        }
    }

    return count;
}