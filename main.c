
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sociedad.h"
#include "simulacion.h"

// muestra los bienes y servicios de una comuna
void mostrar_existencias(struct Comuna *comuna) {
    struct Existencia *actual;

    printf("\ncomuna: %s\n", comuna->nombre);
    printf("poblacion: %d\n", comuna->poblacion);

    // muestra los bienes disponibles
    printf("\nbienes:\n");
    actual = comuna->bienes;

    while (actual != NULL) {
        printf("%s: %d/%d\n",
               actual->nombre,
               actual->cantidad,
               actual->maximo);

        actual = actual->siguiente;
    }

    // muestra los servicios disponibles
    printf("\nservicios:\n");
    actual = comuna->servicios;

    while (actual != NULL) {
        printf("%s: %d/%d\n",
               actual->nombre,
               actual->cantidad,
               actual->maximo);

        actual = actual->siguiente;
    }

    printf("\nnecesidad: %.2f%%\n", comuna->necesidad);
    printf("satisfaccion: %.2f%%\n", comuna->satisfaccion);
}

// ejecuta la prueba del consumo diario
int main(void) {
    struct Sociedad sociedad;

    // prepara los numeros aleatorios
    srand(time(NULL));

    // carga los datos de los archivos
    if (!inicializar_sociedad(&sociedad,
                              "Datos/personas",
                              "Datos/comunas",
                              "Datos/bienes",
                              "Datos/servicios")) {
        printf("\nno se pudo inicializar la sociedad\n");
        return 1;
    }

    // muestra los datos generales
    printf("\n--- sociedad inicializada ---\n");
    printf("poblacion total: %d\n", sociedad.poblacionTotal);
    printf("cantidad de comunas: %d\n", sociedad.cantidadComunas);
    printf("bienes activos: %d\n", sociedad.cantidadBienesActivos);
    printf("servicios activos: %d\n", sociedad.cantidadServiciosActivos);
    printf("dia actual: %d\n", sociedad.dia);

    // muestra las existencias iniciales de la primera comuna
    if (sociedad.comunas != NULL) {
        printf("\n--- antes del consumo ---\n");
        mostrar_existencias(sociedad.comunas);

        // aplica el consumo de 5 dias
        for (int i = 0; i < 5; i++) {
            aplicar_consumo_diario(&sociedad);
        }

        // muestra las existencias despues del consumo
        printf("\n--- despues del consumo ---\n");
        mostrar_existencias(sociedad.comunas);

        printf("\ndia actual: %d\n", sociedad.dia);
    } else {
        printf("\nno hay comunas para probar el consumo\n");
    }

    // libera la memoria utilizada por la sociedad
    liberar_sociedad(&sociedad);

    return 0;
}
