CFLAGS= -g -std=c11 -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700

all: bruh

run:
	./main

bruh:
	cc $(CFLAGS) ./src/main.c ./src/shell.c -o main

clean:
	@rm -f $(OBJ) main.o myshell *~

