
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sociedad.h"
#include "consumo.h"

// muestra las existencias de una comuna
void mostrar_existencias(struct Comuna *comuna) {
    struct Existencia *actual;

    if (comuna == NULL) {
        return;
    }

    printf("\ncomuna: %s\n", comuna->nombre);

    printf("bienes:\n");
    actual = comuna->bienes;

    while (actual != NULL) {
        printf("- %s: %d\n", actual->nombre, actual->cantidad);
        actual = actual->siguiente;
    }

    printf("servicios:\n");
    actual = comuna->servicios;

    while (actual != NULL) {
        printf("- %s: %d\n", actual->nombre, actual->cantidad);
        actual = actual->siguiente;
    }

    printf("necesidad: %.2f\n", comuna->necesidad);
    printf("satisfaccion: %.2f\n", comuna->satisfaccion);
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

    printf("sociedad inicializada correctamente\n");
    printf("cantidad de comunas: %d\n", sociedad.cantidadComunas);

    // ejecuta la simulacion durante diez dias
    for (dia = 0; dia < 10; dia++) {
        printf("\n========== dia %d ==========\n", dia + 1);

        aplicar_consumo_diario(&sociedad);

        // muestra los cambios en la primera comuna
        mostrar_existencias(sociedad.comunas);
    }

    // libera la memoria utilizada
    liberar_sociedad(&sociedad);

    return 0;
}
