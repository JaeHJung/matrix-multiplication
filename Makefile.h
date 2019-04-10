
FLAGS = -Wall -Werror
INCLUDES = ../include

clean:
	rm -f matrix

matrix:
	gcc -o matrix.c matrix -I

newMatrix:
	gcc -lpthread -o newMatrix.c newMatrix.c -I 
	
arrayCreate:
	gcc -lpthread -o arrayCreate.c arrayCreate -I