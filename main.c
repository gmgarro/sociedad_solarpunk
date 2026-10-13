
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
            printf("- %s: %d\n",
                   actual->nombre, actual->cantidad);
            actual = actual->siguiente;
        }

        // muestra los servicios disponibles
        printf("\nservicios:\n");
        actual = comuna->servicios;

        while (actual != NULL) {
            printf("- %s: %d\n",
                   actual->nombre, actual->cantidad);
            actual = actual->siguiente;
        }

        printf("\nnecesidad: %.2f\n", comuna->necesidad);
        printf("satisfaccion: %.2f\n", comuna->satisfaccion);

        comuna = comuna->siguiente;
    }

    printf("\n----------------------------------\n");
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

    printf("\n===== estado inicial =====\n");
    mostrar_comunas(&sociedad);
    // Prueba un trueque entre las dos primeras comunas
    if (sociedad.comunas != NULL &&
    sociedad.comunas->siguiente != NULL) {

    realizar_trueque(sociedad.comunas,
                     sociedad.comunas->siguiente,
                     "Computadora portátil",
                     "Teléfono celular",
                     10);

    mostrar_comunas(&sociedad);
}

    // ejecuta la simulacion durante diez dias
    for (dia = 1; dia <= 10; dia++) {
        printf("\n\n========== dia %d ==========\n", dia);

        aplicar_consumo_diario(&sociedad);

        // muestra el estado de todas las comunas
        mostrar_comunas(&sociedad);
    }

    // libera la memoria utilizada
    liberar_sociedad(&sociedad);

    return 0;
}
