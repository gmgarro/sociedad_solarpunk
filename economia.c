#include <stdio.h>
#include <string.h>
#include "economia.h"

// Busca un bien en la lista de existencias
static struct Existencia *buscar_bien(
    struct Existencia *lista, const char *nombre) {

    while (lista != NULL) {
        if (strcmp(lista->nombre, nombre) == 0) {
            return lista;
        }

        lista = lista->siguiente;
    }

    return NULL;
}

// Realiza un intercambio de bienes entre dos comunas
int realizar_trueque(struct Comuna *origen,
                     struct Comuna *destino,
                     const char *bienOrigen,
                     const char *bienDestino,
                     int cantidad) {

    struct Existencia *entregaOrigen;
    struct Existencia *entregaDestino;
    struct Existencia *recibeOrigen;
    struct Existencia *recibeDestino;

    // Valida los datos del trueque
    if (origen == NULL || destino == NULL ||
    bienOrigen == NULL || bienDestino == NULL ||
    origen == destino || cantidad <= 0 ||
    strcmp(bienOrigen, bienDestino) == 0) {
    printf("Datos del trueque no validos.\n");
    return 0;
    }

    // Busca los bienes que se van a intercambiar
    entregaOrigen = buscar_bien(origen->bienes, bienOrigen);
    entregaDestino = buscar_bien(destino->bienes, bienDestino);

    recibeOrigen = buscar_bien(origen->bienes, bienDestino);
    recibeDestino = buscar_bien(destino->bienes, bienOrigen);

    // Comprueba que los cuatro bienes existan
    if (entregaOrigen == NULL || entregaDestino == NULL ||
        recibeOrigen == NULL || recibeDestino == NULL) {
        printf("No se encontraron todos los bienes necesarios.\n");
        return 0;
    }

    // Comprueba que ambas comunas tengan suficiente cantidad
    if (entregaOrigen->cantidad < cantidad ||
        entregaDestino->cantidad < cantidad) {
        printf("No hay suficientes existencias.\n");
        return 0;
    }

    // Comprueba que las comunas puedan recibir los bienes
    if (recibeOrigen->cantidad + cantidad > recibeOrigen->maximo ||
        recibeDestino->cantidad + cantidad > recibeDestino->maximo) {
        printf("El trueque supera la capacidad maxima.\n");
        return 0;
    }

    // Cada comuna entrega un bien y recibe el de la otra
    entregaOrigen->cantidad -= cantidad;
    recibeOrigen->cantidad += cantidad;

    entregaDestino->cantidad -= cantidad;
    recibeDestino->cantidad += cantidad;

    // Actualiza los indices despues del intercambio
    calcular_indices_comuna(origen);
    calcular_indices_comuna(destino);

    printf("Trueque realizado entre %s y %s.\n",
           origen->nombre, destino->nombre);

    return 1;
}
