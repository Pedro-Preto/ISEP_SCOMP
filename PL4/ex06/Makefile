INCLUDES = 
SOURCES = main.c
OBJFILES = main.o
EXEC = exe

.SUFFIXES : .c .o

.c.o:
		gcc -Wall -g -c $<

${EXEC}: ${OBJFILES}
		gcc -Wall -g -o ${EXEC} ${OBJFILES} -lrt -lpthread

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: ${EXEC}
		./${EXEC}

clean:
		rm -f ${OBJFILES} ${EXEC}