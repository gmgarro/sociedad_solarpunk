#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sociedad.h"
#include "consumo.h"
#include "economia.h"

// muestra las existencias de todas las comunas
void mostrar_comunas(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    struct Existencia *actual;

    // recorre todas las comunas
    while (comuna != NULL) {
        printf("\n----------------------------------\n");
        printf("comuna: %s\n", comuna->nombre);
        printf("poblacion: %d\n", comuna->poblacion);

        // muestra los bienes disponibles
        printf("\nbienes:\n");
        actual = comuna->bienes;

        while (actual != NULL) {
            printf("- %s: cantidad = %d, valor = %.2f\n",
                   actual->nombre, actual->cantidad, actual->valor);
            actual = actual->siguiente;
        }

        // muestra los servicios disponibles
        printf("\nservicios:\n");
        actual = comuna->servicios;

        while (actual != NULL) {
            printf("- %s: cantidad = %d, valor = %.2f\n",
                   actual->nombre, actual->cantidad, actual->valor);
            actual = actual->siguiente;
        }

        printf("\nnecesidad: %.2f\n", comuna->necesidad);
        printf("satisfaccion: %.2f\n", comuna->satisfaccion);

        comuna = comuna->siguiente;
    }

    printf("\n----------------------------------\n");
}

// permite seleccionar un bien o servicio por numero
struct Existencia *seleccionar_existencia(
    struct Existencia *lista, const char *mensaje) {

    struct Existencia *actual = lista;
    struct Existencia *seleccionado;
    int opcion;
    int i = 1;

    if (lista == NULL) {
        printf("No hay elementos disponibles.\n");
        return NULL;
    }

    printf("\n%s\n", mensaje);

    while (actual != NULL) {
        printf("%d. %s (cantidad: %d)\n",
               i, actual->nombre, actual->cantidad);
        actual = actual->siguiente;
        i++;
    }

    printf("Opcion: ");

    if (scanf("%d", &opcion) != 1 || opcion < 1 || opcion >= i) {
        printf("Opcion no valida.\n");
        return NULL;
    }

    seleccionado = lista;

    for (i = 1; i < opcion; i++) {
        seleccionado = seleccionado->siguiente;
    }

    return seleccionado;
}

void menu_trueque(struct Sociedad *sociedad) {
    struct Comuna *origen;
    struct Comuna *destino;
    struct Existencia *elementoOrigen;
    struct Existencia *elementoDestino;
    struct Existencia *listaOrigen;
    struct Existencia *listaDestino;
    int tipo;
    int numero;
    int i;
    int cantidad;

    printf("\n¿Desea realizar un trueque?\n");
    printf("1. Bienes\n2. Servicios\n0. Omitir\nOpcion: ");

    if (scanf("%d", &tipo) != 1 || tipo < 0 || tipo > 2) {
        printf("Opcion no valida.\n");
        return;
    }

    if (tipo == 0) {
        return;
    }

    printf("\nComunas disponibles:\n");
    origen = sociedad->comunas;
    i = 1;

    while (origen != NULL) {
        printf("%d. %s\n", i++, origen->nombre);
        origen = origen->siguiente;
    }

    printf("Numero de la comuna que entrega: ");

    if (scanf("%d", &numero) != 1 || numero < 1) {
        printf("Comuna no valida.\n");
        return;
    }

    origen = sociedad->comunas;

    for (i = 1; origen != NULL && i < numero; i++) {
        origen = origen->siguiente;
    }

    if (origen == NULL) {
        printf("Comuna no valida.\n");
        return;
    }

    printf("Numero de la comuna que recibe: ");

    if (scanf("%d", &numero) != 1 || numero < 1) {
        printf("Comuna no valida.\n");
        return;
    }

    destino = sociedad->comunas;

    for (i = 1; destino != NULL && i < numero; i++) {
        destino = destino->siguiente;
    }

    if (destino == NULL || destino == origen) {
        printf("Comuna no valida.\n");
        return;
    }

    // selecciona las listas segun el tipo de trueque
    if (tipo == 1) {
        listaOrigen = origen->bienes;
        listaDestino = destino->bienes;
    } else {
        listaOrigen = origen->servicios;
        listaDestino = destino->servicios;
    }

    // selecciona el elemento que entrega la comuna de origen
    elementoOrigen = seleccionar_existencia(
        listaOrigen, "Seleccione el elemento que entrega:");

    if (elementoOrigen == NULL) {
        return;
    }

    // selecciona el elemento que entrega la comuna de destino
    elementoDestino = seleccionar_existencia(
        listaDestino, "Seleccione el elemento que recibe:");

    if (elementoDestino == NULL) {
        return;
    }

    printf("Cantidad a intercambiar: ");

    if (scanf("%d", &cantidad) != 1 || cantidad <= 0) {
        printf("Cantidad no valida.\n");
        return;
    }

    if (tipo == 1) {
        realizar_trueque(origen, destino,
                         elementoOrigen->nombre,
                         elementoDestino->nombre,
                         cantidad);
    } else {
        realizar_trueque_servicios(origen, destino,
                                   elementoOrigen->nombre,
                                   elementoDestino->nombre,
                                   cantidad);
    }
}

// inicia la sociedad y ejecuta la simulacion
int main(void) {
    struct Sociedad sociedad;
    int dia;

    // inicia los numeros aleatorios
    srand(time(NULL));

    // carga los datos de los archivos
    if (!inicializar_sociedad(&sociedad,
                              "Datos/personas",
                              "Datos/comunas",
                              "Datos/bienes",
                              "Datos/servicios")) {
        printf("no se pudo inicializar la sociedad\n");
        return 1;
    }

    calcular_indices_sociedad(&sociedad);

    printf("\n===== estado inicial =====\n");
    mostrar_comunas(&sociedad);
    menu_trueque(&sociedad);
    calcular_indices_sociedad(&sociedad);
    mostrar_comunas(&sociedad);

    // ejecuta la simulacion durante diez dias
    for (dia = 1; dia <= 10; dia++) {
        printf("\n\n========== dia %d ==========\n", dia);

        menu_trueque(&sociedad);
        aplicar_consumo_diario(&sociedad);
        calcular_indices_sociedad(&sociedad);
        mostrar_comunas(&sociedad);
    }

    return 0;
}
