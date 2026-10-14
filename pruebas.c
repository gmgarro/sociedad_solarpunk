#include <stdio.h>
#include <string.h>
#include "listas.h"
#include "sociedad.h"

// estas funciones estan en sociedad.c y se usan para probar la lista doble
int agregar_bien(struct Sociedad *sociedad, const char *nombre);
int agregar_comuna(struct Sociedad *sociedad, const char *nombre);

// cuenta cuantas pruebas salieron bien y cuantas mal
int pruebasOk = 0;
int pruebasMal = 0;

// muestra si una prueba salio bien o mal y la cuenta
void revisar(const char *descripcion, int condicion) {
    if (condicion) {
        printf("  [ok]  %s\n", descripcion);
        pruebasOk++;
    } else {
        printf("  [MAL] %s\n", descripcion);
        pruebasMal++;
    }
}

// deja una sociedad vacia para poder probar las listas
void sociedad_vacia(struct Sociedad *sociedad) {
    sociedad->poblacionTotal = 0;
    sociedad->cantidadComunas = 0;
    sociedad->cantidadBienesActivos = 0;
    sociedad->cantidadServiciosActivos = 0;
    sociedad->dia = 0;
    sociedad->diasParaEmergencia = 0;
    sociedad->personas = NULL;
    sociedad->catalogoServicios = NULL;
    sociedad->catalogoBienes = NULL;
    sociedad->nombresComunas = NULL;
    sociedad->comunas = NULL;
}

// prueba la lista simple de listas.c
void probar_lista_simple(void) {
    Lista lista;

    printf("\nlista simple (listas.c)\n");

    inicializar_lista(&lista);
    revisar("la lista nueva esta vacia", lista.inicio == NULL &&
            lista.final == NULL && lista.cantidad == 0);

    insertar_final(&lista, "ana");
    revisar("con un solo nodo, inicio y final son el mismo",
            lista.inicio == lista.final && lista.cantidad == 1);

    insertar_final(&lista, "luis");
    insertar_final(&lista, "maria");
    revisar("despues de 3 inserciones la cantidad es 3",
            lista.cantidad == 3);
    revisar("el inicio es ana", strcmp(lista.inicio->nombre, "ana") == 0);
    revisar("el final es maria", strcmp(lista.final->nombre, "maria") == 0);
    revisar("el orden es ana, luis, maria",
            strcmp(lista.inicio->siguiente->nombre, "luis") == 0 &&
            lista.inicio->siguiente->siguiente == lista.final);
    revisar("el final apunta a NULL", lista.final->siguiente == NULL);

    printf("  contenido de la lista:\n");
    imprimir_lista(&lista);

    liberar_lista(&lista);
    revisar("despues de liberar la lista queda vacia",
            lista.inicio == NULL && lista.final == NULL &&
            lista.cantidad == 0);
}

// prueba la lista doble que se usa en el catalogo de bienes
void probar_lista_doble_bienes(void) {
    struct Sociedad sociedad;
    struct NodoNombreDoble *primero;
    struct NodoNombreDoble *segundo;
    struct NodoNombreDoble *ultimo;
    struct NodoNombreDoble *actual;

    printf("\nlista doble (catalogo de bienes)\n");

    sociedad_vacia(&sociedad);
    agregar_bien(&sociedad, "pan");
    agregar_bien(&sociedad, "agua");
    agregar_bien(&sociedad, "semillas");

    primero = sociedad.catalogoBienes;
    segundo = primero->siguiente;
    ultimo = segundo->siguiente;

    revisar("el primero no tiene anterior", primero->anterior == NULL);
    revisar("el ultimo no tiene siguiente", ultimo->siguiente == NULL);
    revisar("el anterior del segundo es el primero",
            segundo->anterior == primero);
    revisar("el anterior del ultimo es el segundo",
            ultimo->anterior == segundo);

    printf("  recorrido hacia adelante: ");
    actual = primero;
    while (actual != NULL) {
        printf("%s ", actual->nombre);
        actual = actual->siguiente;
    }
    printf("\n");

    printf("  recorrido hacia atras:    ");
    actual = ultimo;
    while (actual != NULL) {
        printf("%s ", actual->nombre);
        actual = actual->anterior;
    }
    printf("\n");

    revisar("el orden es pan, agua, semillas",
            strcmp(primero->nombre, "pan") == 0 &&
            strcmp(segundo->nombre, "agua") == 0 &&
            strcmp(ultimo->nombre, "semillas") == 0);

    liberar_sociedad(&sociedad);
    revisar("despues de liberar el catalogo queda vacio",
            sociedad.catalogoBienes == NULL);
}

// prueba la lista de comunas (con puntero anterior) y la lista doble de nombres
void probar_comunas(void) {
    struct Sociedad sociedad;
    struct Comuna *primera;
    struct Comuna *segunda;
    struct Comuna *tercera;
    struct NodoNombreDoble *nombre;

    printf("\ncomunas (lista con anterior y siguiente)\n");

    sociedad_vacia(&sociedad);
    agregar_comuna(&sociedad, "Santiago");
    agregar_comuna(&sociedad, "Providencia");
    agregar_comuna(&sociedad, "Las Condes");

    primera = sociedad.comunas;
    segunda = primera->siguiente;
    tercera = segunda->siguiente;

    revisar("la primera comuna no tiene anterior", primera->anterior == NULL);
    revisar("la ultima comuna no tiene siguiente", tercera->siguiente == NULL);
    revisar("el anterior de la segunda es la primera",
            segunda->anterior == primera);
    revisar("el anterior de la tercera es la segunda",
            tercera->anterior == segunda);

    // la lista doble de nombres tiene el mismo orden
    nombre = sociedad.nombresComunas;
    revisar("la lista de nombres empieza en Santiago",
            strcmp(nombre->nombre, "Santiago") == 0);
    nombre = nombre->siguiente->siguiente;
    revisar("la lista de nombres termina en Las Condes",
            strcmp(nombre->nombre, "Las Condes") == 0);
    revisar("yendo hacia atras desde el final se llega a Providencia",
            strcmp(nombre->anterior->nombre, "Providencia") == 0);

    liberar_sociedad(&sociedad);
    revisar("despues de liberar no quedan comunas", sociedad.comunas == NULL);
}

// ejecuta todas las pruebas y muestra el resumen
int main(void) {
    probar_lista_simple();
    probar_lista_doble_bienes();
    probar_comunas();

    printf("\nresumen: %d pruebas bien, %d pruebas mal\n",
           pruebasOk, pruebasMal);

    if (pruebasMal > 0) {
        return 1;
    }

    return 0;
}