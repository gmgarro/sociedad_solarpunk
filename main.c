/*
#include <stdio.h>
#include "listas.h"
#include "archivos.h"

// carga los archivos y muestra los datos guardados
int main(void) {
    Lista personas;
    Lista servicios;
    Lista bienes;
    Lista comunas;

    // inicializa las listas
    inicializar_lista(&personas);
    inicializar_lista(&servicios);
    inicializar_lista(&bienes);
    inicializar_lista(&comunas);

    // carga los datos de cada archivo
    int resultado = cargar_archivo("Datos/personas", &personas);
    resultado = cargar_archivo("Datos/servicios", &servicios) && resultado;
    resultado = cargar_archivo("Datos/bienes", &bienes) && resultado;
    resultado = cargar_archivo("Datos/comunas", &comunas) && resultado;

    // muestra los datos si todos los archivos se cargaron bien
    if (resultado) {
        printf("personas: %d\n", personas.cantidad);
        printf("servicios: %d\n", servicios.cantidad);
        printf("bienes: %d\n", bienes.cantidad);
        printf("comunas: %d\n", comunas.cantidad);
    }

    // libera la memoria de las listas
    liberar_lista(&personas);
    liberar_lista(&servicios);
    liberar_lista(&bienes);
    liberar_lista(&comunas);

    return resultado ? 0 : 1;
}
*/


#include <stdio.h>
#include "sociedad.h"

// cuenta los nombres de una lista simple
int contar_nombres(struct NodoNombre *lista) {
    int cantidad = 0;

    while (lista != NULL) {
        cantidad++;
        lista = lista->siguiente;
    }

    return cantidad;
}

// cuenta los nombres de una lista doble
int contar_bienes(struct NodoNombreDoble *lista) {
    int cantidad = 0;

    while (lista != NULL) {
        cantidad++;
        lista = lista->siguiente;
    }

    return cantidad;
}

// carga los datos y muestra la informacion de la sociedad
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

    printf("Personas: %d\n", sociedad.poblacionTotal);
    printf("Comunas: %d\n", sociedad.cantidadComunas);

    printf("Bienes: %d\n",
           contar_bienes(sociedad.catalogoBienes));

    printf("Servicios: %d\n",
           contar_nombres(sociedad.catalogoServicios));

    liberar_sociedad(&sociedad);

    return 0;
}

