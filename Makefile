
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

programa: main.c listas.c archivos.c sociedad.c
	$(CC) $(CFLAGS) main.c listas.c archivos.c sociedad.c -o programa

clean:
	rm -f programa *.o
