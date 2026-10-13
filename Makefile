CC = gcc
CFLAGS = -Wall -Wextra -std=c11

programa: main.c listas.c archivos.c sociedad.c consumo.c emergencias.c economia.c
	$(CC) $(CFLAGS) main.c listas.c archivos.c sociedad.c consumo.c emergencias.c economia.c -o programa

clean:
	rm -f programa *.o