
#include <stdio.h>
#include <stdlib.h>
#include "emergencias.h"
#include "sociedad.h"


 // genera una emergencia en una comuna aleatoria
void generar_emergencia(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    struct Comuna *afectada;
    struct Existencia *bien;
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

    // selecciona una comuna al azar
    posicion = rand() % cantidadComunas;
    afectada = sociedad->comunas;

    for (i = 0; i < posicion; i++) {
        afectada = afectada->siguiente;
    }

    printf("\n¡emergencia en la comuna %s!\n", afectada->nombre);

    // reduce el 25 por ciento de cada bien
    bien = afectada->bienes;

    while (bien != NULL) {
        int reduccion = bien->cantidad / 4;

        bien->cantidad -= reduccion;

        printf("%s: se perdieron %d unidades\n",
               bien->nombre, reduccion);

        bien = bien->siguiente;
    }

    // actualiza la necesidad y satisfaccion de la comuna afectada
    calcular_indices_comuna(afectada);

    // reduce la satisfaccion de las comunas cercanas
    afectar_comunas_cercanas(sociedad, afectada);
}


// reduce la satisfaccion de las comunas cercanas a una emergencia
void afectar_comunas_cercanas(struct Sociedad *sociedad,
                              struct Comuna *afectada) {
    struct Comuna *actual = sociedad->comunas;
    int posicion = 0;
    int posicionAfectada = 0;
    int encontrada = 0;

    // busca la posicion de la comuna afectada
    while (actual != NULL) {
        if (actual == afectada) {
            posicionAfectada = posicion;
            encontrada = 1;
            break;
        }

        actual = actual->siguiente;
        posicion++;
    }

    // revisa si encontro la comuna
    if (!encontrada) {
        return;
    }

    actual = sociedad->comunas;
    posicion = 0;

    // busca las comunas que estan a una o dos posiciones
    while (actual != NULL) {
        int distancia = posicion - posicionAfectada;

        if (distancia < 0) {
            distancia = -distancia;
        }

        if (distancia >= 1 && distancia <= 2) {
            actual->penalizacionEmergencia += 10;

            printf("la comuna %s tambien fue afectada por la emergencia\n",
                   actual->nombre);
        }

        // avanza siempre, aunque la comuna no sea afectada
        actual = actual->siguiente;
        posicion++;
    }
}