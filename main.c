
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sociedad.h"
#include "consumo.h"
#include "economia.h"

// colores para organizar la informacion en la terminal
#define VERDE "\033[32m"
#define ROJO "\033[31m"
#define AMARILLO "\033[33m"
#define CIAN "\033[36m"
#define AZUL "\033[34m"
#define NEGRITA "\033[1m"
#define RESET "\033[0m"

// muestra un titulo para separar las secciones
void mostrar_titulo(const char *titulo) {
    printf("\n" CIAN NEGRITA);
    printf("=======================================================\n");
    printf("  %s\n", titulo);
    printf("=======================================================\n");
    printf(RESET);
}

// muestra un mensaje de error
void mostrar_error(const char *mensaje) {
    printf(ROJO "[!] %s\n" RESET, mensaje);
}

// muestra un mensaje exitoso
void mostrar_exito(const char *mensaje) {
    printf(VERDE "[+] %s\n" RESET, mensaje);
}

// limpia los caracteres que quedan cuando se escribe una opcion incorrecta
void limpiar_entrada(void) {
    int caracter;

    while ((caracter = getchar()) != '\n' && caracter != EOF) {
    }
}

// muestra las existencias de todas las comunas
void mostrar_comunas(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    struct Existencia *actual;

    while (comuna != NULL) {
        printf("\n" AZUL "-------------------------------------------------------\n" RESET);
        printf(NEGRITA "  COMUNA: %s\n" RESET, comuna->nombre);
        printf("  Poblacion: %d personas\n", comuna->poblacion);

        printf("\n" VERDE "  BIENES\n" RESET);
        actual = comuna->bienes;

        if (actual == NULL) {
            printf("  No hay bienes registrados.\n");
        }

        while (actual != NULL) {
            printf("  - %-22s cantidad: %-5d valor: %.2f\n",
                   actual->nombre, actual->cantidad, actual->valor);
            actual = actual->siguiente;
        }

        printf("\n" CIAN "  SERVICIOS\n" RESET);
        actual = comuna->servicios;

        if (actual == NULL) {
            printf("  No hay servicios registrados.\n");
        }

        while (actual != NULL) {
            printf("  - %-22s cantidad: %-5d valor: %.2f\n",
                   actual->nombre, actual->cantidad, actual->valor);
            actual = actual->siguiente;
        }

        printf("\n  Necesidad: ");
        if (comuna->necesidad >= 70) {
            printf(ROJO "%.2f\n" RESET, comuna->necesidad);
        } else if (comuna->necesidad >= 40) {
            printf(AMARILLO "%.2f\n" RESET, comuna->necesidad);
        } else {
            printf(VERDE "%.2f\n" RESET, comuna->necesidad);
        }

        printf("  Satisfaccion: ");
        if (comuna->satisfaccion >= 70) {
            printf(VERDE "%.2f\n" RESET, comuna->satisfaccion);
        } else if (comuna->satisfaccion >= 40) {
            printf(AMARILLO "%.2f\n" RESET, comuna->satisfaccion);
        } else {
            printf(ROJO "%.2f\n" RESET, comuna->satisfaccion);
        }

        comuna = comuna->siguiente;
    }

    printf(AZUL "-------------------------------------------------------\n" RESET);
}

// permite seleccionar un bien o servicio por numero
struct Existencia *seleccionar_existencia(
    struct Existencia *lista, const char *mensaje) {

    struct Existencia *actual = lista;
    struct Existencia *seleccionado;
    int opcion;
    int i = 1;

    if (lista == NULL) {
        mostrar_error("No hay elementos disponibles.");
        return NULL;
    }

    printf("\n" NEGRITA "%s\n" RESET, mensaje);

    while (actual != NULL) {
        printf("  %d. %-22s cantidad: %d, valor: %.2f\n",
               i, actual->nombre, actual->cantidad, actual->valor);
        actual = actual->siguiente;
        i++;
    }

    printf("\nSeleccione una opcion: ");

    if (scanf("%d", &opcion) != 1) {
        limpiar_entrada();
        mostrar_error("Debe ingresar un numero.");
        return NULL;
    }

    if (opcion < 1 || opcion >= i) {
        mostrar_error("La opcion seleccionada no existe.");
        return NULL;
    }

    seleccionado = lista;

    for (i = 1; i < opcion; i++) {
        seleccionado = seleccionado->siguiente;
    }

    return seleccionado;
}

// permite realizar un trueque entre dos comunas
void menu_trueque(struct Sociedad *sociedad) {
    struct Comuna *origen;
    struct Comuna *destino;
    struct Existencia *elementoOrigen;
    struct Existencia *elementoDestino;
    struct Existencia *listaOrigen;
    struct Existencia *listaDestino;
    int tipo;
    int numero;
    int i;
    int cantidad;

    mostrar_titulo("MENU DE TRUEQUE");

    printf("Seleccione el tipo de intercambio:\n\n");
    printf("  " VERDE "1." RESET " Intercambiar bienes\n");
    printf("  " CIAN "2." RESET " Intercambiar servicios\n");
    printf("  " AMARILLO "0." RESET " Continuar sin intercambiar\n\n");
    printf("Opcion: ");

    if (scanf("%d", &tipo) != 1) {
        limpiar_entrada();
        mostrar_error("Debe ingresar un numero.");
        return;
    }

    if (tipo < 0 || tipo > 2) {
        mostrar_error("La opcion no es valida.");
        return;
    }

    if (tipo == 0) {
        printf(AMARILLO "\nNo se realizara ningun trueque.\n" RESET);
        return;
    }

    // muestra las comunas disponibles
    printf("\n" NEGRITA "COMUNAS DISPONIBLES\n" RESET);
    origen = sociedad->comunas;
    i = 1;

    while (origen != NULL) {
        printf("  %d. %s\n", i++, origen->nombre);
        origen = origen->siguiente;
    }

    printf("\nNumero de la comuna que entrega: ");

    if (scanf("%d", &numero) != 1) {
        limpiar_entrada();
        mostrar_error("Debe ingresar un numero.");
        return;
    }

    if (numero < 1) {
        mostrar_error("La comuna seleccionada no existe.");
        return;
    }

    origen = sociedad->comunas;

    for (i = 1; origen != NULL && i < numero; i++) {
        origen = origen->siguiente;
    }

    if (origen == NULL) {
        mostrar_error("La comuna seleccionada no existe.");
        return;
    }

    printf("Numero de la comuna que recibe: ");

    if (scanf("%d", &numero) != 1) {
        limpiar_entrada();
        mostrar_error("Debe ingresar un numero.");
        return;
    }

    if (numero < 1) {
        mostrar_error("La comuna seleccionada no existe.");
        return;
    }

    destino = sociedad->comunas;

    for (i = 1; destino != NULL && i < numero; i++) {
        destino = destino->siguiente;
    }

    if (destino == NULL || destino == origen) {
        mostrar_error("Seleccione una comuna diferente a la de origen.");
        return;
    }

    // selecciona las listas segun el tipo de trueque
    if (tipo == 1) {
        listaOrigen = origen->bienes;
        listaDestino = destino->bienes;
    } else {
        listaOrigen = origen->servicios;
        listaDestino = destino->servicios;
    }

    printf("\n" AZUL "Origen: %s\n" RESET, origen->nombre);
    elementoOrigen = seleccionar_existencia(
        listaOrigen, "Seleccione lo que entrega esta comuna:");

    if (elementoOrigen == NULL) {
        return;
    }

    printf("\n" AZUL "Destino: %s\n" RESET, destino->nombre);
    elementoDestino = seleccionar_existencia(
        listaDestino, "Seleccione lo que entrega la otra comuna:");

    if (elementoDestino == NULL) {
        return;
    }

    printf("\nCantidad que intercambiara cada comuna: ");

    if (scanf("%d", &cantidad) != 1) {
        limpiar_entrada();
        mostrar_error("Debe ingresar una cantidad numerica.");
        return;
    }

    if (cantidad <= 0) {
        mostrar_error("La cantidad debe ser mayor que cero.");
        return;
    }

    // realiza el intercambio seleccionado
    if (tipo == 1) {
        if (realizar_trueque(origen, destino,
                             elementoOrigen->nombre,
                             elementoDestino->nombre,
                             cantidad)) {
            mostrar_exito("El intercambio de bienes se completo.");
        }
    } else {
        if (realizar_trueque_servicios(origen, destino,
                                       elementoOrigen->nombre,
                                       elementoDestino->nombre,
                                       cantidad)) {
            mostrar_exito("El intercambio de servicios se completo.");
        }
    }
}

// muestra un resumen de la sociedad al finalizar la simulacion
void mostrar_resumen(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    float sumaSatisfaccion = 0;
    float promedioSatisfaccion = 0;
    float sumaNecesidad = 0;
    int cantidadComunas = 0;

    // suma los indices de todas las comunas
    while (comuna != NULL) {
        sumaSatisfaccion += comuna->satisfaccion;
        sumaNecesidad += comuna->necesidad;
        cantidadComunas++;
        comuna = comuna->siguiente;
    }

    if (cantidadComunas > 0) {
        promedioSatisfaccion = sumaSatisfaccion / cantidadComunas;
        sumaNecesidad /= cantidadComunas;
    }

    mostrar_titulo("RESUMEN FINAL DE LA SOCIEDAD");

    printf("Dias simulados:       %d\n", sociedad->dia);
    printf("Poblacion total:      %d personas\n", sociedad->poblacionTotal);
    printf("Comunas activas:      %d\n", cantidadComunas);

    printf("\nPromedio de satisfaccion: ");
    if (promedioSatisfaccion >= 70) {
        printf(VERDE "%.2f / 100\n" RESET, promedioSatisfaccion);
    } else if (promedioSatisfaccion >= 40) {
        printf(AMARILLO "%.2f / 100\n" RESET, promedioSatisfaccion);
    } else {
        printf(ROJO "%.2f / 100\n" RESET, promedioSatisfaccion);
    }

    printf("Promedio de necesidad:    %.2f / 100\n", sumaNecesidad);

    printf("\n" NEGRITA "Estado final por comuna\n" RESET);
    comuna = sociedad->comunas;

    while (comuna != NULL) {
        printf("  %-20s satisfaccion: %6.2f | necesidad: %6.2f\n",
               comuna->nombre, comuna->satisfaccion, comuna->necesidad);
        comuna = comuna->siguiente;
    }

    printf("\n");
    if (promedioSatisfaccion >= 70) {
        printf(VERDE "La sociedad termina con una satisfaccion favorable.\n" RESET);
    } else if (promedioSatisfaccion >= 40) {
        printf(AMARILLO "La sociedad termina con resultados intermedios.\n" RESET);
    } else {
        printf(ROJO "La sociedad termina con una satisfaccion baja.\n" RESET);
    }
}

// inicia la sociedad y ejecuta la simulacion
int main(void) {
    struct Sociedad sociedad;
    int dia;

    // inicia los numeros aleatorios
    srand(time(NULL));

    mostrar_titulo("SIMULACION DE LA SOCIEDAD SOLARPUNK");
    printf("Administre los intercambios y observe los cambios\n");
    printf("en las necesidades y la satisfaccion de las comunas.\n");

    // carga los datos de los archivos
    if (!inicializar_sociedad(&sociedad,
                              "Datos/personas",
                              "Datos/comunas",
                              "Datos/bienes",
                              "Datos/servicios")) {
        mostrar_error("No se pudo inicializar la sociedad.");
        return 1;
    }

    calcular_indices_sociedad(&sociedad);

    mostrar_titulo("ESTADO INICIAL");
    mostrar_comunas(&sociedad);

    // ejecuta la simulacion durante quince dias
    for (dia = 1; dia <= 15; dia++) {
        char tituloDia[40];

        snprintf(tituloDia, sizeof(tituloDia), "DIA %d DE 15", dia);
        mostrar_titulo(tituloDia);

        menu_trueque(&sociedad);
        aplicar_consumo_diario(&sociedad);
        calcular_indices_sociedad(&sociedad);

        printf("\n");
        mostrar_exito("El consumo diario se ha aplicado.");
        mostrar_comunas(&sociedad);
    }

    // presenta los resultados finales
    mostrar_resumen(&sociedad);

    // libera toda la memoria antes de terminar
    liberar_sociedad(&sociedad);

    mostrar_exito("La simulacion ha terminado correctamente.");

    return 0;
}
