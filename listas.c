
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listas.h"

// deja la lista vacía y pone la cantidad de nodos en cero
void inicializar_lista(Lista *lista) {
    lista->inicio = NULL;
    lista->final = NULL;
    lista->cantidad = 0;
}

// crea un nodo con el nombre y lo agrega al final de la lista
int insertar_final(Lista *lista, const char *nombre) {
    Nodo *nuevo = malloc(sizeof(Nodo));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->nombre = malloc(strlen(nombre) + 1);

    if (nuevo->nombre == NULL) {
        free(nuevo);
        return 0;
    }

    strcpy(nuevo->nombre, nombre);
    nuevo->siguiente = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = nuevo;
    } else {
        lista->final->siguiente = nuevo;
    }

    lista->final = nuevo;
    lista->cantidad++;

    return 1;
}

// recorre los nodos desde el inicio e imprime cada nombre
void imprimir_lista(const Lista *lista) {
    Nodo *actual = lista->inicio;

    while (actual != NULL) {
        printf("%s\n", actual->nombre);
        actual = actual->siguiente;
    }
}

// recorre la lista, libera la memoria y la deja vacía
void liberar_lista(Lista *lista) {
    Nodo *actual = lista->inicio;

    while (actual != NULL) {
        Nodo *siguiente = actual->siguiente;

        free(actual->nombre);
        free(actual);

        actual = siguiente;
    }

    inicializar_lista(lista);
}
