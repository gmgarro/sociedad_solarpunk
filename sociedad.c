
#include "sociedad.h"
#include "listas.h"
#include "archivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hace una copia del nombre para guardarlo en memoria
char *copiar_texto(const char *texto) {
    char *copia = malloc(strlen(texto) + 1);

    if (copia == NULL) {
        return NULL;
    }

    strcpy(copia, texto);
    return copia;
}

// Agrega una persona a la lista
int agregar_persona(struct Sociedad *sociedad, const char *nombre) {
    struct NodoNombre *nuevo = malloc(sizeof(struct NodoNombre));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->nombre = copiar_texto(nombre);

    if (nuevo->nombre == NULL) {
        free(nuevo);
        return 0;
    }

    nuevo->siguiente = NULL;

    if (sociedad->personas == NULL) {
        sociedad->personas = nuevo;
    } else {
        struct NodoNombre *actual = sociedad->personas;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }

    return 1;
}

// Agrega un bien al catalogo de bienes
int agregar_bien(struct Sociedad *sociedad, const char *nombre) {
    struct NodoNombreDoble *nuevo = malloc(sizeof(struct NodoNombreDoble));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->nombre = copiar_texto(nombre);

    if (nuevo->nombre == NULL) {
        free(nuevo);
        return 0;
    }

    nuevo->anterior = NULL;
    nuevo->siguiente = NULL;

    if (sociedad->catalogoBienes == NULL) {
        sociedad->catalogoBienes = nuevo;
    } else {
        struct NodoNombreDoble *actual = sociedad->catalogoBienes;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
        nuevo->anterior = actual;
    }

    return 1;
}

// Agrega un servicio al catalogo de servicios
int agregar_servicio(struct Sociedad *sociedad, const char *nombre) {
    struct NodoNombre *nuevo = malloc(sizeof(struct NodoNombre));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->nombre = copiar_texto(nombre);

    if (nuevo->nombre == NULL) {
        free(nuevo);
        return 0;
    }

    nuevo->siguiente = NULL;

    if (sociedad->catalogoServicios == NULL) {
        sociedad->catalogoServicios = nuevo;
    } else {
        struct NodoNombre *actual = sociedad->catalogoServicios;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }

    return 1;
}

// Agrega una comuna y guarda su nombre en la lista doble
int agregar_comuna(struct Sociedad *sociedad, const char *nombre) {
    struct Comuna *nueva = malloc(sizeof(struct Comuna));

    if (nueva == NULL) {
        return 0;
    }

    nueva->nombre = copiar_texto(nombre);

    if (nueva->nombre == NULL) {
        free(nueva);
        return 0;
    }

    nueva->poblacion = 0;
    nueva->bienes = NULL;
    nueva->servicios = NULL;
    nueva->necesidad = 0;
    nueva->satisfaccion = 0;
    nueva->siguiente = NULL;

    // Agrega la comuna a la lista de comunas
    if (sociedad->comunas == NULL) {
        sociedad->comunas = nueva;
    } else {
        struct Comuna *actual = sociedad->comunas;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nueva;
    }

    // Guarda tambien el nombre en la lista doble
    struct NodoNombreDoble *nuevoNombre;
    nuevoNombre = malloc(sizeof(struct NodoNombreDoble));

    if (nuevoNombre == NULL) {
        return 0;
    }

    nuevoNombre->nombre = copiar_texto(nombre);

    if (nuevoNombre->nombre == NULL) {
        free(nuevoNombre);
        return 0;
    }

    nuevoNombre->anterior = NULL;
    nuevoNombre->siguiente = NULL;

    if (sociedad->nombresComunas == NULL) {
        sociedad->nombresComunas = nuevoNombre;
    } else {
        struct NodoNombreDoble *actual = sociedad->nombresComunas;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevoNombre;
        nuevoNombre->anterior = actual;
    }

    return 1;
}

// Carga los datos iniciales de la sociedad
int inicializar_sociedad(struct Sociedad *sociedad,
                         const char *archivoPersonas,
                         const char *archivoComunas,
                         const char *archivoBienes,
                         const char *archivoServicios) {

    // Deja la sociedad vacia antes de empezar
    sociedad->poblacionTotal = 0;
    sociedad->cantidadComunas = 0;
    sociedad->cantidadBienesActivos = 0;
    sociedad->cantidadServiciosActivos = 0;
    sociedad->dia = 0;

    sociedad->personas = NULL;
    sociedad->catalogoServicios = NULL;
    sociedad->catalogoBienes = NULL;
    sociedad->nombresComunas = NULL;
    sociedad->comunas = NULL;

    // Listas temporales para leer los archivos
    Lista personas;
    Lista comunas;
    Lista bienes;
    Lista servicios;

    inicializar_lista(&personas);
    inicializar_lista(&comunas);
    inicializar_lista(&bienes);
    inicializar_lista(&servicios);

    // Lee los cuatro archivos
    if (!cargar_archivo(archivoPersonas, &personas) ||
        !cargar_archivo(archivoComunas, &comunas) ||
        !cargar_archivo(archivoBienes, &bienes) ||
        !cargar_archivo(archivoServicios, &servicios)) {

        liberar_lista(&personas);
        liberar_lista(&comunas);
        liberar_lista(&bienes);
        liberar_lista(&servicios);
        return 0;
    }

    // Copia los nombres de las personas
    Nodo *actual = personas.inicio;

    while (actual != NULL) {
        if (!agregar_persona(sociedad, actual->nombre)) {
            liberar_lista(&personas);
            liberar_lista(&comunas);
            liberar_lista(&bienes);
            liberar_lista(&servicios);
            liberar_sociedad(sociedad);
            return 0;
        }

        sociedad->poblacionTotal++;
        actual = actual->siguiente;
    }

    // Crea las comunas
    actual = comunas.inicio;

    while (actual != NULL) {
        if (!agregar_comuna(sociedad, actual->nombre)) {
            liberar_lista(&personas);
            liberar_lista(&comunas);
            liberar_lista(&bienes);
            liberar_lista(&servicios);
            liberar_sociedad(sociedad);
            return 0;
        }

        sociedad->cantidadComunas++;
        actual = actual->siguiente;
    }

    // Copia los bienes disponibles en el catalogo
    actual = bienes.inicio;

    while (actual != NULL) {
        if (!agregar_bien(sociedad, actual->nombre)) {
            liberar_lista(&personas);
            liberar_lista(&comunas);
            liberar_lista(&bienes);
            liberar_lista(&servicios);
            liberar_sociedad(sociedad);
            return 0;
        }

        actual = actual->siguiente;
    }

    // Copia los servicios disponibles en el catalogo
    actual = servicios.inicio;

    while (actual != NULL) {
        if (!agregar_servicio(sociedad, actual->nombre)) {
            liberar_lista(&personas);
            liberar_lista(&comunas);
            liberar_lista(&bienes);
            liberar_lista(&servicios);
            liberar_sociedad(sociedad);
            return 0;
        }

        actual = actual->siguiente;
    }

    // Libera las listas temporales porque ya copiamos los datos
    liberar_lista(&personas);
    liberar_lista(&comunas);
    liberar_lista(&bienes);
    liberar_lista(&servicios);

    return 1;
}

// Libera toda la memoria de la sociedad
void liberar_sociedad(struct Sociedad *sociedad) {
    struct NodoNombre *persona;
    struct NodoNombreDoble *bien;
    struct NodoNombreDoble *nombreComuna;
    struct Comuna *comuna;

    while (sociedad->personas != NULL) {
        persona = sociedad->personas;
        sociedad->personas = persona->siguiente;

        free(persona->nombre);
        free(persona);
    }

    while (sociedad->catalogoServicios != NULL) {
        persona = sociedad->catalogoServicios;
        sociedad->catalogoServicios = persona->siguiente;

        free(persona->nombre);
        free(persona);
    }

    while (sociedad->catalogoBienes != NULL) {
        bien = sociedad->catalogoBienes;
        sociedad->catalogoBienes = bien->siguiente;

        free(bien->nombre);
        free(bien);
    }

    while (sociedad->nombresComunas != NULL) {
        nombreComuna = sociedad->nombresComunas;
        sociedad->nombresComunas = nombreComuna->siguiente;

        free(nombreComuna->nombre);
        free(nombreComuna);
    }

    while (sociedad->comunas != NULL) {
        comuna = sociedad->comunas;
        sociedad->comunas = comuna->siguiente;

        free(comuna->nombre);
        free(comuna);
    }
}
