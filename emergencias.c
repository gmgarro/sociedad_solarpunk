
#include <stdio.h>
#include <stdlib.h>
#include "emergencias.h"
#include "sociedad.h"

// quita el 25 % de la cantidad de cada bien o servicio de una lista
static void perder_cuarto(struct Existencia *lista) {
    struct Existencia *actual = lista;

    while (actual != NULL) {
        int reduccion = actual->cantidad / 4;

        // solo muestra lo que de verdad se perdio
        if (reduccion > 0) {
            actual->cantidad -= reduccion;

            printf("%s: se perdieron %d unidades\n",
                   actual->nombre, reduccion);
        }

        actual = actual->siguiente;
    }
}

// genera una emergencia en una comuna aleatoria
void generar_emergencia(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    struct Comuna *afectada;
    int cantidadComunas = 0;
    int posicion;
    int i;

    if(sociedad == NULL)
    {
        return;
    }

    comuna = sociedad->comunas;

    // cuenta las comunas disponibles
    while (comuna != NULL) {
        cantidadComunas++;
        comuna = comuna->siguiente;
    }

    // revisa si hay comunas disponibles
    if (cantidadComunas == 0) {
        return;
    }

    // selecciona una comuna al azar
    posicion = rand() % cantidadComunas;
    afectada = sociedad->comunas;

    for (i = 0; i < posicion; i++) {
        afectada = afectada->siguiente;
    }

    printf("\n¡emergencia en la comuna %s!\n", afectada->nombre);

    // la comuna afectada pierde el 25 % de sus bienes y servicios
    printf("bienes:\n");
    perder_cuarto(afectada->bienes);

    printf("servicios:\n");
    perder_cuarto(afectada->servicios);

    // la satisfaccion de la comuna afectada baja directo
    afectada->penalizacionEmergencia += 20;

    // actualiza la necesidad y satisfaccion de la comuna afectada
    calcular_indices_comuna(afectada);

    // reduce la satisfaccion de las comunas cercanas
    afectar_comunas_cercanas(afectada);
}

// baja la satisfaccion de una comuna cercana (si existe)
static void penalizar_cercana(struct Comuna *cercana) {
    if (cercana == NULL) {
        return;
    }

    cercana->penalizacionEmergencia += 10;

    // actualiza la necesidad y satisfaccion
    calcular_indices_comuna(cercana);

    printf("la comuna %s tambien fue afectada por la emergencia\n",
           cercana->nombre);
}

// las cercanas son la anterior, la siguiente y la siguiente de la siguiente
void afectar_comunas_cercanas(struct Comuna *afectada) {
    penalizar_cercana(afectada->anterior);
    penalizar_cercana(afectada->siguiente);

    if (afectada->siguiente != NULL) {
        penalizar_cercana(afectada->siguiente->siguiente);
    }
}