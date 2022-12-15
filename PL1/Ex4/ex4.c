#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
 int a=0, b, c, d; 
 b = (int) fork(); 
 c = (int) getpid(); /* getpid(), getppid(): unistd.h*/ 
 d = (int) getppid(); 
 a = a + 5; 
printf("\n a=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
 }
