#include <fcntl.h> /* For constants O_* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int number[10];
    char stringIsep[10][20];
} Share;


#define MY_SHARED_FILE "/EX03_SHARED_FILE"
#define MY_DATA_SIZE sizeof(Share)

#define MY_SHM_OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define MY_SHM_OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define MY_SHM_OPEN_READER_OFLAG O_EXCL | O_RDWR
#define MY_SHM_OPEN_READER_MODE S_IRUSR | S_IWUSR