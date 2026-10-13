
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
