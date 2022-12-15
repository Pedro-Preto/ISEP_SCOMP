NAME=ex4

${NAME}: ${NAME}.o
	gcc -Wall -g ${NAME}.o -o ${NAME} -lrt -lpthread
	
${NAME}.o: ${NAME}.c
	gcc -Wall -g -c ${NAME}.c -o ${NAME}.o
	
run: ${NAME}
	./${NAME}
	
all: ${NAME}
	./${NAME}
	
clean:
	rm ${NAME}.o ${NAME}