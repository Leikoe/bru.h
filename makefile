CFLAGS= -g -std=c11 -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700

all: bruh

bruh: main.o

main.o:
	cc $(CFLAGS) ./src/main.c

clean:
	@rm -f $(OBJ) main.o myshell *~

