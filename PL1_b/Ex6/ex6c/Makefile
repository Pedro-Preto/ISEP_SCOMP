# variaveis do make

NAME = ex6c
SOURCES = ${NAME}.c
OBJFILES = ${NAME}.o
EXE = ${NAME}

# para fazer o programa principal
${EXE}: ${OBJFILES}
	gcc -Wall -g ${OBJFILES} -o ${EXE}
	
# execução do programa principal 
run: ${EXE}
	./${EXE}

# limpeza 
clean:
	rm ${OBJFILES} ${EXE}
	
