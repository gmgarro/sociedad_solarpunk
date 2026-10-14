
#include "sociedad.h"
#include "listas.h"
#include "archivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hace una copia del nombre para guardarlo en memoria
char *copiar_texto(const char *texto) {
    char *copia = malloc(strlen(texto) + 1);

    if (copia == NULL) {
        return NULL;
    }

    strcpy(copia, texto);
    return copia;
}

// agrega una persona a la lista
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

// agrega un bien al catalogo de bienes
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

// agrega un servicio al catalogo de servicios
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


 // agrega una comuna y guarda su nombre en la lista doble
int agregar_comuna(struct Sociedad *sociedad, const char *nombre) {
    struct Comuna *nueva = malloc(sizeof(struct Comuna));
    struct NodoNombreDoble *nuevoNombre;

    if (nueva == NULL) {
        return 0;
    }

    nueva->nombre = copiar_texto(nombre);

    if (nueva->nombre == NULL) {
        free(nueva);
        return 0;
    }

    // inicializa los datos de la comuna
    nueva->poblacion = 0;
    nueva->bienes = NULL;
    nueva->servicios = NULL;
    nueva->necesidad = 0;
    nueva->satisfaccion = 0;
    nueva->penalizacionEmergencia = 0;
    nueva->anterior = NULL;
    nueva->siguiente = NULL;

    // reserva memoria para guardar tambien el nombre
    nuevoNombre = malloc(sizeof(struct NodoNombreDoble));

    if (nuevoNombre == NULL) {
        free(nueva->nombre);
        free(nueva);
        return 0;
    }

    nuevoNombre->nombre = copiar_texto(nombre);

    if (nuevoNombre->nombre == NULL) {
        free(nuevoNombre);
        free(nueva->nombre);
        free(nueva);
        return 0;
    }

    nuevoNombre->anterior = NULL;
    nuevoNombre->siguiente = NULL;

    // agrega la comuna a la lista principal
    if (sociedad->comunas == NULL) {
        sociedad->comunas = nueva;
    } else {
        struct Comuna *actual = sociedad->comunas;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nueva;
        nueva->anterior = actual;
    }

    // agrega el nombre a la lista doble
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



// agrega un bien o servicio a las existencias de una comuna
int agregar_existencia(struct Existencia **lista, const char *nombre,
                       int maximo) {
    struct Existencia *nuevo = malloc(sizeof(struct Existencia));

    if (nuevo == NULL) {
        return 0;
    }

    nuevo->nombre = copiar_texto(nombre);

    if (nuevo->nombre == NULL) {
        free(nuevo);
        return 0;
    }

    nuevo->maximo = maximo;
    nuevo->cantidad = maximo * 4 / 5;
    nuevo->valor = 1.0f;
    nuevo->siguiente = NULL;

    if (*lista == NULL) {
        *lista = nuevo;
    } else {
        struct Existencia *actual = *lista;

        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }

    return 1;
}

// crea las existencias de una comuna
int crear_existencias_comuna(struct Sociedad *sociedad,
                             struct Comuna *comuna) {
    struct NodoNombreDoble *bien = sociedad->catalogoBienes;
    struct NodoNombre *servicio = sociedad->catalogoServicios;
    int i;

    for (i = 0; i < sociedad->cantidadBienesActivos; i++) {
        if (bien == NULL) {
            return 0;
        }

        if (!agregar_existencia(&comuna->bienes, bien->nombre,
                                comuna->poblacion * 5)) {
            return 0;
        }

        bien = bien->siguiente;
    }

    for (i = 0; i < sociedad->cantidadServiciosActivos; i++) {
        if (servicio == NULL) {
            return 0;
        }

        if (!agregar_existencia(&comuna->servicios, servicio->nombre,
                                comuna->poblacion * 2)) {
            return 0;
        }

        servicio = servicio->siguiente;
    }

    return 1;
}

// calcula la necesidad y la satisfaccion de una comuna
void calcular_indices_comuna(struct Comuna *comuna) {
    struct Existencia *actual;
    int maximoTotal = 0;
    int cantidadTotal = 0;

    if (comuna == NULL) {
        return;
    }

    actual = comuna->bienes;
    while (actual != NULL) {
        maximoTotal += actual->maximo;
        cantidadTotal += actual->cantidad;
        actual = actual->siguiente;
    }

    actual = comuna->servicios;
    while (actual != NULL) {
        maximoTotal += actual->maximo;
        cantidadTotal += actual->cantidad;
        actual = actual->siguiente;
    }

    if (maximoTotal > 0) {
        comuna->necesidad =
            100.0f * (maximoTotal - cantidadTotal) / maximoTotal;
    } else {
        comuna->necesidad = 0;
    }

    if (comuna->necesidad < 0) {
        comuna->necesidad = 0;
    }

    if (comuna->necesidad >= 100) {
        comuna->necesidad = 99.9f;
    }

    comuna->satisfaccion =
    100.0f - comuna->necesidad
    - comuna->penalizacionEmergencia;

    if (comuna->satisfaccion < 0) {
        comuna->satisfaccion = 0;
    }
}


// calcula los indices de todas las comunas y mezcla la satisfaccion con la de las cercanas
void calcular_indices_sociedad(struct Sociedad *sociedad) {
    struct Comuna *actual;
    struct Comuna *vecina;
    float bienestarBase[50];
    int cantidad = 0;
    int i;
    int j;

    if (sociedad == NULL) {
        return;
    }

    actual = sociedad->comunas;

    // calcula los indices propios de cada comuna
    while (actual != NULL && cantidad < 50) {
        calcular_indices_comuna(actual);

        bienestarBase[cantidad] =
            100.0f - actual->necesidad
            - actual->penalizacionEmergencia;

        if (bienestarBase[cantidad] < 0) {
            bienestarBase[cantidad] = 0;
        }

        if (bienestarBase[cantidad] > 100) {
            bienestarBase[cantidad] = 100;
        }

        actual = actual->siguiente;
        cantidad++;
    }

    // combina el bienestar propio con el de las comunas cercanas
    actual = sociedad->comunas;

    for (i = 0; actual != NULL && i < cantidad; i++) {
        float sumaVecinas = 0;
        int cantidadVecinas = 0;

        vecina = sociedad->comunas;

        for (j = 0; vecina != NULL && j < cantidad; j++) {
            int distancia = i - j;

            if (distancia < 0) {
                distancia = -distancia;
            }

            if (distancia >= 1 && distancia <= 2) {
                sumaVecinas += bienestarBase[j];
                cantidadVecinas++;
            }

            vecina = vecina->siguiente;
        }

        if (cantidadVecinas > 0) {
            float bienestarPropio = bienestarBase[i];
            float promedioVecinas = sumaVecinas / cantidadVecinas;

            actual->satisfaccion =
                0.7f * bienestarPropio
                + 0.3f * promedioVecinas;
        } else {
            actual->satisfaccion =
                100.0f - actual->necesidad
                - actual->penalizacionEmergencia;
        }

        if (actual->satisfaccion < 0) {
            actual->satisfaccion = 0;
        }

        if (actual->satisfaccion > 100) {
            actual->satisfaccion = 100;
        }

        actual = actual->siguiente;
    }
}

// carga los datos iniciales de la sociedad
int inicializar_sociedad(struct Sociedad *sociedad,
                         const char *archivoPersonas,
                         const char *archivoComunas,
                         const char *archivoBienes,
                         const char *archivoServicios) {

    // deja la sociedad vacia antes de empezar
    sociedad->poblacionTotal = 0;
    sociedad->cantidadComunas = 0;
    sociedad->cantidadBienesActivos = 0;
    sociedad->cantidadServiciosActivos = 0;
    sociedad->dia = 0;
    sociedad->diasParaEmergencia = (rand() % 5) + 3;

    sociedad->personas = NULL;
    sociedad->catalogoServicios = NULL;
    sociedad->catalogoBienes = NULL;
    sociedad->nombresComunas = NULL;
    sociedad->comunas = NULL;

    // listas temporales para leer los archivos
    Lista personas;
    Lista comunas;
    Lista bienes;
    Lista servicios;

    inicializar_lista(&personas);
    inicializar_lista(&comunas);
    inicializar_lista(&bienes);
    inicializar_lista(&servicios);

    // lee los cuatro archivos
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

    // copia los nombres de las personas
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

    // crea las comunas
    
    int comunasActivas;
    int bienesActivos;
    int serviciosActivos;

    printf("Cantidad de comunas activas (1-%d): ",
           comunas.cantidad);

    if (scanf("%d", &comunasActivas) != 1 ||
        comunasActivas < 1 ||
        comunasActivas > comunas.cantidad) {
        printf("Cantidad de comunas no valida\n");
        liberar_lista(&personas);
        liberar_lista(&comunas);
        liberar_lista(&bienes);
        liberar_lista(&servicios);
        liberar_sociedad(sociedad);
        return 0;
    }

    printf("Cantidad de bienes activos (1-%d): ",
           bienes.cantidad);

    if (scanf("%d", &bienesActivos) != 1 ||
        bienesActivos < 1 ||
        bienesActivos > bienes.cantidad) {
        printf("Cantidad de bienes no valida\n");
        liberar_lista(&personas);
        liberar_lista(&comunas);
        liberar_lista(&bienes);
        liberar_lista(&servicios);
        liberar_sociedad(sociedad);
        return 0;
    }

    printf("Cantidad de servicios activos (1-%d): ",
           servicios.cantidad);

    if (scanf("%d", &serviciosActivos) != 1 ||
        serviciosActivos < 1 ||
        serviciosActivos > servicios.cantidad) {
        printf("Cantidad de servicios no valida\n");
        liberar_lista(&personas);
        liberar_lista(&comunas);
        liberar_lista(&bienes);
        liberar_lista(&servicios);
        liberar_sociedad(sociedad);
        return 0;
    }

    sociedad->cantidadBienesActivos = bienesActivos;
    sociedad->cantidadServiciosActivos = serviciosActivos;

    // copia los bienes del catalogo
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

    // copia los servicios del catalogo
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

    // crea solamente las comunas seleccionadas
    actual = comunas.inicio;

    while (actual != NULL && sociedad->cantidadComunas <
                              comunasActivas) {
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

    // reparte la poblacion entre las comunas
    struct Comuna *comunaActual = sociedad->comunas;
    int poblacionBase = sociedad->poblacionTotal / comunasActivas;
    int personasExtra = sociedad->poblacionTotal % comunasActivas;
    int numeroComuna = 0;

    while (comunaActual != NULL) {
        comunaActual->poblacion = poblacionBase;

        if (numeroComuna < personasExtra) {
            comunaActual->poblacion++;
        }

        if (!crear_existencias_comuna(sociedad, comunaActual)) {
            liberar_lista(&personas);
            liberar_lista(&comunas);
            liberar_lista(&bienes);
            liberar_lista(&servicios);
            liberar_sociedad(sociedad);
            return 0;
        }

        calcular_indices_comuna(comunaActual);

        numeroComuna++;
        comunaActual = comunaActual->siguiente;
    }


    // libera las listas temporales porque ya copiamos los datos
    liberar_lista(&personas);
    liberar_lista(&comunas);
    liberar_lista(&bienes);
    liberar_lista(&servicios);

    return 1;
}


// libera una lista de existencias
void liberar_existencias(struct Existencia *lista) {
    struct Existencia *actual;

    while (lista != NULL) {
        actual = lista;
        lista = lista->siguiente;

        free(actual->nombre);
        free(actual);
    }
}

// libera toda la memoria de la sociedad
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
        liberar_existencias(comuna->bienes);
        liberar_existencias(comuna->servicios);

        free(comuna->nombre);
        free(comuna);
    }
}