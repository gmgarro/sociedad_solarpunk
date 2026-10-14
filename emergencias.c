
#include <stdio.h>
#include <stdlib.h>
#include "emergencias.h"
#include "sociedad.h"

// colores para mostrar los mensajes
#define ROJO "\033[31m"
#define AMARILLO "\033[33m"
#define CIAN "\033[36m"
#define VERDE "\033[32m"
#define NEGRITA "\033[1m"
#define RESET "\033[0m"

// quita el 25 % de la cantidad de cada bien o servicio de una lista
static void perder_cuarto(struct Existencia *lista) {
    struct Existencia *actual = lista;
    int reduccion;
    int huboPerdidas = 0;

    while (actual != NULL) {
        reduccion = actual->cantidad / 4;

        // solo muestra lo que de verdad se perdio
        if (reduccion > 0) {
            actual->cantidad -= reduccion;

            printf("  - %-30s " ROJO "-%d unidades\n" RESET,
                   actual->nombre, reduccion);

            huboPerdidas = 1;
        }

        actual = actual->siguiente;
    }

    if (!huboPerdidas) {
        printf("  " VERDE "No hubo perdidas en esta categoria.\n" RESET);
    }
}

// genera una emergencia en una comuna aleatoria
void generar_emergencia(struct Sociedad *sociedad) {
    struct Comuna *comuna;
    struct Comuna *afectada;
    int cantidadComunas = 0;
    int posicion;
    int i;

    if (sociedad == NULL) {
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

    // muestra el aviso principal de la emergencia
    printf("\n");
    printf(AMARILLO NEGRITA);
    printf("==================================================\n");
    printf("                 ALERTA DEL SISTEMA\n");
    printf("==================================================\n");
    printf(RESET);

    printf(ROJO NEGRITA);
    printf("\n  ¡EMERGENCIA EN LA COMUNA %s!\n", afectada->nombre);
    printf(RESET);

    printf("\nSe ha registrado una emergencia que afecta a esta comuna.\n");
    printf("Se aplicaran perdidas en sus bienes y servicios.\n");

    // muestra las perdidas de bienes
    printf("\n" CIAN NEGRITA "  PERDIDAS DE BIENES\n" RESET);
    printf("  ------------------------------------------------\n");
    perder_cuarto(afectada->bienes);

    // muestra las perdidas de servicios
    printf("\n" CIAN NEGRITA "  PERDIDAS DE SERVICIOS\n" RESET);
    printf("  ------------------------------------------------\n");
    perder_cuarto(afectada->servicios);

    // la satisfaccion de la comuna afectada baja directo
    afectada->penalizacionEmergencia += 20;

    // actualiza la necesidad y satisfaccion de la comuna afectada
    calcular_indices_comuna(afectada);

    // reduce la satisfaccion de las comunas cercanas
    afectar_comunas_cercanas(afectada);

    printf("\n" AMARILLO NEGRITA);
    printf("  La emergencia tambien puede afectar a las comunas cercanas.\n");
    printf("==================================================\n");
    printf(RESET);
}

// baja la satisfaccion de una comuna cercana (si existe)
static void penalizar_cercana(struct Comuna *cercana) {
    if (cercana == NULL) {
        return;
    }

    cercana->penalizacionEmergencia += 10;

    // actualiza la necesidad y satisfaccion
    calcular_indices_comuna(cercana);

    printf(AMARILLO "  [!] La comuna %s tambien fue afectada.\n" RESET,
           cercana->nombre);
}

// las cercanas son la anterior, la siguiente y la siguiente de la siguiente
void afectar_comunas_cercanas(struct Comuna *afectada) {
    if (afectada == NULL) {
        return;
    }

    penalizar_cercana(afectada->anterior);
    penalizar_cercana(afectada->siguiente);

    if (afectada->siguiente != NULL) {
        penalizar_cercana(afectada->siguiente->siguiente);
    }
}
