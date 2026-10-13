#ifndef SOCIEDAD_H
#define SOCIEDAD_H

// Guarda un nombre en una lista simple
struct NodoNombre {
    char *nombre;
    struct NodoNombre *siguiente;
};

// Esta lista tiene enlaces hacia adelante y hacia atrás
struct NodoNombreDoble {
    char *nombre;
    struct NodoNombreDoble *anterior;
    struct NodoNombreDoble *siguiente;
};

// Guarda las existencias de un bien o servicio en una comuna
struct Existencia {
    char *nombre;
    int cantidad;
    int maximo;
    struct Existencia *siguiente;
};

// Representa una comuna dentro de la sociedad
struct Comuna {
    char *nombre;
    int poblacion;

    struct Existencia *bienes;
    struct Existencia *servicios;

    float necesidad;
    float satisfaccion;

    struct Comuna *siguiente;
};

// Contiene los datos generales de la simulacion
struct Sociedad {
    int poblacionTotal;
    int cantidadComunas;
    int cantidadBienesActivos;
    int cantidadServiciosActivos;
    int dia;

    struct NodoNombre *personas;
    struct NodoNombre *catalogoServicios;
    struct NodoNombreDoble *catalogoBienes;
    struct NodoNombreDoble *nombresComunas;

    struct Comuna *comunas;
};

int inicializar_sociedad(struct Sociedad *sociedad,
                         const char *archivoPersonas,
                         const char *archivoComunas,
                         const char *archivoBienes,
                         const char *archivoServicios);

void liberar_sociedad(struct Sociedad *sociedad);

//calcula los indices de necesidad y satisfaccion de una comuna
void calcular_indices_comuna(struct Comuna *comuna);

#endif
