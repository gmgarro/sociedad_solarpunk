
#include <stdio.h>
#include <stdlib.h>
#include "simulacion.h"

// reduce una cantidad variable de las existencias
void consumir_existencias(struct Existencia *lista) {
    struct Existencia *actual = lista;

    while (actual != NULL) {
        // genera un consumo entre 1 y 5 unidades
        int consumo = rand() % 5 + 1;

        // evita que la cantidad quede por debajo de cero
        if (consumo > actual->cantidad) {
            consumo = actual->cantidad;
        }

        actual->cantidad -= consumo;
        actual = actual->siguiente;
    }
}

// aplica el consumo de un dia en todas las comunas
void aplicar_consumo_diario(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;

    // recorre todas las comunas
    while (comuna != NULL) {
        consumir_existencias(comuna->bienes);
        consumir_existencias(comuna->servicios);

        // actualiza la necesidad y satisfaccion
        calcular_indices_comuna(comuna);

        comuna = comuna->siguiente;
    }

    // avanza un dia en la simulacion
    sociedad->dia++;

    // genera una emergencia cada cinco dias
    if (sociedad->dia % 5 == 0) {
        generar_emergencia(sociedad);
    }
}

// genera una emergencia en una comuna aleatoria
void generar_emergencia(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    struct Comuna *afectada;
    int cantidadComunas = 0;
    int posicion;
    int i;

    // cuenta las comunas disponibles
    while (comuna != NULL) {
        cantidadComunas++;
        comuna = comuna->siguiente;
    }

    // revisa si hay comunas disponibles
    if (cantidadComunas == 0) {
        return;
    }

    // elige una comuna al azar
    posicion = rand() % cantidadComunas;
    afectada = sociedad->comunas;

    for (i = 0; i < posicion; i++) {
        afectada = afectada->siguiente;
    }

    printf("\n¡emergencia en la comuna %s!\n", afectada->nombre);

    // reduce el 25 por ciento de cada bien
    struct Existencia *bien = afectada->bienes;

    while (bien != NULL) {
        int reduccion = bien->cantidad / 4;

        bien->cantidad -= reduccion;

        printf("%s: se perdieron %d unidades\n",
               bien->nombre, reduccion);

        bien = bien->siguiente;
    }

    // actualiza la necesidad y satisfaccion de la comuna
    calcular_indices_comuna(afectada);
}
