
FLAGS = -Wall -Werror
INCLUDES = ../include

clean:
	rm -f matrix

matrix:
	gcc -o matrix.c matrix -I

newMatrix:
	gcc -o newMatrix.c newMatrix.c -I
	
arrayCreate:
	gcc -o arrayCreate.c arrayCreate -I