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

typedef struct {
    int number[10][100];
    int max[10];
} shared_data_type;

int main(){
//data to be shared
    shared_data_type *shared_data;
    
    int totalNum[1000];
    srand((unsigned) time(NULL) * getpid()); 

    //carregar os dados para o array de 1000 integers
    for(int i=0;i<1000;i++){
    totalNum[i]=(rand() % 1001);
    //printf("%d\n",totalNum[i]) ;
    }

    //size of data
    int size = sizeof(shared_data);

    // Creates and opens a shared memory area
    int fd = shm_open("/ex9", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

//carregar os dados para a memoria partilhada
int position=0;
           for(int i=0;i<1000;i++){
                if(i==99||i==199||i==299||i==399||i==499||i==599||i==699||i==799||i==899||i==999){
                    position++;
                }
                shared_data->number[position][i]=totalNum[i];
            }

int id=spawnChilds(10);

    for(int i=1;i<11;i++){
        if(id==i){
        //cada filho analiza 100 posições
        int max=0;
            for(int j=((i-1)*100);j<((i*100)-1);j++){
                if(shared_data->number[i-1][j]>=max){
                    max=shared_data->number[i-1][j];
                }
            }
        shared_data->max[i-1]=max;
        exit(EXIT_SUCCESS);
        }
    }

    for(int i=0;i<10;i++){
        wait(NULL);
    }

	printf("============Max Value=============\n");
		for(int i=0;i<10;i++){
		printf("Maior Numero da %dº Parte do array:%d\n",i+1,shared_data->max[i]);

	}
	printf("======================================\n");

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
   if (shm_unlink("/ex9") < 0) {
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

