#Makefile

INCLUDES = ex.c
SOURCES = ex.c
OBJFILES = ex.o
EXEC = ex

.SUFIXES : .c .o

.c.o:
	gcc -g -Wall -c $<

${EXEC}: ${OBJFILES}
	gcc -g -Wall -o ${EXEC} ${OBJFILES}

${OBJFILES}: ${SOURCES} ${INCLUDES}

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}