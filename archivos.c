
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "archivos.h"

// lee un archivo y guarda sus nombres en una lista
int cargar_archivo(const char *nombre_archivo, Lista *lista) {
    FILE *archivo = fopen(nombre_archivo, "r");

    // revisa si el archivo se pudo abrir
    if (archivo == NULL) {
        printf("no se pudo abrir el archivo: %s\n", nombre_archivo);
        return 0;
    }

    char *linea = NULL;
    size_t capacidad = 0;
    ssize_t longitud;
    int resultado = 1;

    // lee el archivo linea por linea
    while ((longitud = getline(&linea, &capacidad, archivo)) != -1) {

        // elimina el salto de linea del texto
        linea[strcspn(linea, "\r\n")] = '\0';

        // guarda el nombre si la linea no esta vacia
        if (strlen(linea) > 0) {
            if (!insertar_final(lista, linea)) {
                printf("no se pudo guardar un nombre en la lista\n");
                resultado = 0;
                break;
            }
        }
    }

    // libera la memoria usada para leer las lineas
    free(linea);

    // cierra el archivo
    fclose(archivo);

    return resultado;
}
