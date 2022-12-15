/*
 * ex2.c
 *
 *  Created on: 10/03/2022
 *      Author: isep
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include<sys/wait.h>

int main(){

	printf("I'm..\n");
	pid_t p = fork();

	if(p == -1){

		printf("Error");
	}

	if( p == 0){

		printf("I'll never join you!\n");
	} else {
		wait(NULL);
		p = fork();

		if(p == 0){

			printf("I'll never join you!\n");
		} else {

			printf("the..\n");
			wait(NULL);
			p = fork();

			if(p == 0){
				printf("I'll never join you!\n");

			} else{
				printf("father!\n");

			}
		}
	}
}

