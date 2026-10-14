#ifndef SOCIEDAD_H
#define SOCIEDAD_H

// guarda un nombre en una lista simple
struct NodoNombre {
    char *nombre;
    struct NodoNombre *siguiente;
};

// esta lista tiene enlaces hacia adelante y hacia atrás
struct NodoNombreDoble {
    char *nombre;
    struct NodoNombreDoble *anterior;
    struct NodoNombreDoble *siguiente;
};

// guarda las existencias de un bien o servicio en una comuna
struct Existencia {
    char *nombre;
    int cantidad;
    int maximo;
    float valor;
    struct Existencia *siguiente;
};

// representa una comuna dentro de la sociedad
struct Comuna {
    char *nombre;
    int poblacion;

    struct Existencia *bienes;
    struct Existencia *servicios;

    float necesidad;
    float satisfaccion;
    float penalizacionEmergencia;

    struct Comuna *anterior;
    struct Comuna *siguiente;
};

// contiene los datos generales de la simulacion
struct Sociedad {
    int poblacionTotal;
    int cantidadComunas;
    int cantidadBienesActivos;
    int cantidadServiciosActivos;
    int dia;
    int diasParaEmergencia;

    struct NodoNombre *personas;
    struct NodoNombre *catalogoServicios;
    struct NodoNombreDoble *catalogoBienes;
    struct NodoNombreDoble *nombresComunas;

    struct Comuna *comunas;
};

// pide los datos al usuario, carga los archivos y crea la sociedad
int inicializar_sociedad(struct Sociedad *sociedad,
                         const char *archivoPersonas,
                         const char *archivoComunas,
                         const char *archivoBienes,
                         const char *archivoServicios);

// libera toda la memoria de la sociedad
void liberar_sociedad(struct Sociedad *sociedad);

// calcula los indices de necesidad y satisfaccion de una comuna
void calcular_indices_comuna(struct Comuna *comuna);
// calcula los indices de todas las comunas de la sociedad
void calcular_indices_sociedad(struct Sociedad *sociedad);
#endif