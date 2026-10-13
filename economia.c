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

// Realiza un intercambio de servicios entre dos comunas
int realizar_trueque_servicios(struct Comuna *origen,
                               struct Comuna *destino,
                               const char *servicioOrigen,
                               const char *servicioDestino,
                               int cantidad) {
    struct Existencia *entregaOrigen;
    struct Existencia *entregaDestino;
    struct Existencia *recibeOrigen;
    struct Existencia *recibeDestino;

    if (origen == NULL || destino == NULL ||
        servicioOrigen == NULL || servicioDestino == NULL ||
        origen == destino || cantidad <= 0 ||
        strcmp(servicioOrigen, servicioDestino) == 0) {
        printf("Datos del trueque no validos.\n");
        return 0;
    }

    entregaOrigen = buscar_bien(origen->servicios, servicioOrigen);
    entregaDestino = buscar_bien(destino->servicios, servicioDestino);
    recibeOrigen = buscar_bien(origen->servicios, servicioDestino);
    recibeDestino = buscar_bien(destino->servicios, servicioOrigen);

    if (entregaOrigen == NULL || entregaDestino == NULL ||
    recibeOrigen == NULL || recibeDestino == NULL) {
    printf("Error buscando servicios para el trueque:\n");
    printf("Servicio de origen '%s': %s\n",
           servicioOrigen,
           entregaOrigen ? "encontrado" : "NO encontrado");
    printf("Servicio de destino '%s': %s\n",
           servicioDestino,
           entregaDestino ? "encontrado" : "NO encontrado");
    printf("Servicio recibido por origen '%s': %s\n",
           servicioDestino,
           recibeOrigen ? "encontrado" : "NO encontrado");
    printf("Servicio recibido por destino '%s': %s\n",
           servicioOrigen,
           recibeDestino ? "encontrado" : "NO encontrado");
    return 0;
}

    if (entregaOrigen->cantidad < cantidad ||
        entregaDestino->cantidad < cantidad) {
        printf("No hay suficientes servicios disponibles.\n");
        return 0;
    }

    if (recibeOrigen->cantidad + cantidad > recibeOrigen->maximo ||
        recibeDestino->cantidad + cantidad > recibeDestino->maximo) {
        printf("El trueque supera la capacidad maxima.\n");
        return 0;
    }

    entregaOrigen->cantidad -= cantidad;
    recibeOrigen->cantidad += cantidad;
    entregaDestino->cantidad -= cantidad;
    recibeDestino->cantidad += cantidad;

    calcular_indices_comuna(origen);
    calcular_indices_comuna(destino);

    printf("Trueque de servicios realizado entre %s y %s.\n",
           origen->nombre, destino->nombre);

    return 1;
}
