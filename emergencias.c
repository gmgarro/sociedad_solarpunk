
#include <stdio.h>
#include <stdlib.h>
#include "emergencias.h"

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

    // actualiza la necesidad y satisfaccion
    calcular_indices_comuna(afectada);
}
