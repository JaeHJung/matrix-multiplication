
FLAGS = -Wall -Werror
INCLUDES = ../include

clean:
	rm -f matrix

matrix:
	gcc -o matrix.c matrix -I

newMatrix:
	gcc newMatrix.c -o newMatrix -lpthread -Wall -Werror -I
	
arrayCreate:
	gcc -lpthread -o arrayCreate.c arrayCreate -I