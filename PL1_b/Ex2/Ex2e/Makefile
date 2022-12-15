exe: exe.o
	gcc -Wall -g exe.o -o exe
	
exe.o: exe.c
	gcc -Wall -g -c exe.c -o exe.o
	
run: exe
	./exe
	
clean:
	rm exe.o exe