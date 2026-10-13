#include <stdio.h>
#include "sociedad.h"

// Muestra los bienes y servicios de una comuna
void mostrar_existencias(char *titulo, struct Existencia *lista) {
    printf("%s:\n", titulo);

    while (lista != NULL) {
        printf("  %s: %d / %d\n",
               lista->nombre, lista->cantidad, lista->maximo);

        lista = lista->siguiente;
    }
}

// Muestra los datos de cada comuna
void mostrar_comunas(struct Comuna *comuna) {
    while (comuna != NULL) {
        printf("\n-----------------------------\n");
        printf("Comuna: %s\n", comuna->nombre);
        printf("Poblacion: %d\n", comuna->poblacion);

        mostrar_existencias("Bienes", comuna->bienes);
        mostrar_existencias("Servicios", comuna->servicios);

        printf("Necesidad: %.2f%%\n", comuna->necesidad);
        printf("Satisfaccion: %.2f%%\n", comuna->satisfaccion);

        comuna = comuna->siguiente;
    }
}

// Carga los datos y muestra la informacion de la sociedad
int main(void) {
    struct Sociedad sociedad;

    int resultado = inicializar_sociedad(
        &sociedad,
        "Datos/personas",
        "Datos/comunas",
        "Datos/bienes",
        "Datos/servicios"
    );

    if (!resultado) {
        printf("Error al cargar los datos\n");
        return 1;
    }

    printf("\n===== SOCIEDAD SOLARPUNK =====\n");
    printf("Personas: %d\n", sociedad.poblacionTotal);
    printf("Comunas activas: %d\n", sociedad.cantidadComunas);
    printf("Bienes activos: %d\n", sociedad.cantidadBienesActivos);
    printf("Servicios activos: %d\n", sociedad.cantidadServiciosActivos);

    mostrar_comunas(sociedad.comunas);

    liberar_sociedad(&sociedad);

    return 0;
}


