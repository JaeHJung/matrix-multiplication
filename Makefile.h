
FLAGS = -Wall -Werror
INCLUDES = ../include

clean:
	rm -f matrix

matrix:
	gcc -o matrix.c matrix -I