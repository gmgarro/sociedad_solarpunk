CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# archivos que usan la simulacion y las pruebas (sin main.c ni pruebas.c)
MODULOS = listas.c archivos.c sociedad.c consumo.c emergencias.c economia.c
ENCABEZADOS = listas.h archivos.h sociedad.h consumo.h emergencias.h economia.h

# compila la simulacion
programa: main.c $(MODULOS) $(ENCABEZADOS)
	$(CC) $(CFLAGS) main.c $(MODULOS) -o programa

# compila las pruebas de las estructuras de datos
pruebas: pruebas.c $(MODULOS) $(ENCABEZADOS)
	$(CC) $(CFLAGS) pruebas.c $(MODULOS) -o pruebas

# borra los archivos que se generan al compilar
clean:
	rm -f programa pruebas *.o

.PHONY: clean