
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sociedad.h"
#include "consumo.h"
#include "economia.h"
#include "emergencias.h"

// colores para mostrar los mensajes
#define VERDE "\033[32m"
#define ROJO "\033[31m"
#define AMARILLO "\033[33m"
#define CIAN "\033[36m"
#define AZUL "\033[34m"
#define NEGRITA "\033[1m"
#define RESET "\033[0m"

// muestra un titulo en pantalla
void mostrar_titulo(const char *titulo) {
    printf("\n" CIAN NEGRITA);
    printf("========================================\n");
    printf("  %s\n", titulo);
    printf("========================================\n");
    printf(RESET);
}

// muestra un mensaje de error
void mostrar_error(const char *mensaje) {
    printf(ROJO "\n[!] %s\n" RESET, mensaje);
}

// muestra un mensaje de exito
void mostrar_exito(const char *mensaje) {
    printf(VERDE "\n[+] %s\n" RESET, mensaje);
}

// limpia los datos que quedaron en la entrada
void limpiar_entrada(void) {
    int caracter;

    while ((caracter = getchar()) != '\n' && caracter != EOF) {
    }
}

// pide un numero dentro de un rango valido
int pedir_opcion(const char *mensaje, int minimo, int maximo) {
    int opcion;

    while (1) {
        printf("%s", mensaje);

        if (scanf("%d", &opcion) != 1) {
            limpiar_entrada();
            mostrar_error("Entrada no valida. Ingrese un numero.");
            continue;
        }

        limpiar_entrada();

        if (opcion < minimo || opcion > maximo) {
            mostrar_error("Opcion fuera de rango. Intente de nuevo.");
            continue;
        }

        return opcion;
    }
}

// muestra el estado actual de las comunas
void mostrar_comunas(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;

    mostrar_titulo("ESTADO DE LAS COMUNAS");

    if (comuna == NULL) {
        mostrar_error("No hay comunas para mostrar.");
        return;
    }

    printf("%-22s %10s %12s %14s\n",
           "Comuna", "Poblacion", "Necesidad", "Satisfaccion");

    printf("--------------------------------------------------------------\n");

    while (comuna != NULL) {
        printf("%-22s %10d ", comuna->nombre, comuna->poblacion);

        if (comuna->necesidad >= 70) {
            printf(ROJO "%12.2f " RESET, comuna->necesidad);
        } else if (comuna->necesidad >= 40) {
            printf(AMARILLO "%12.2f " RESET, comuna->necesidad);
        } else {
            printf(VERDE "%12.2f " RESET, comuna->necesidad);
        }

        if (comuna->satisfaccion >= 70) {
            printf(VERDE "%14.2f\n" RESET, comuna->satisfaccion);
        } else if (comuna->satisfaccion >= 40) {
            printf(AMARILLO "%14.2f\n" RESET, comuna->satisfaccion);
        } else {
            printf(ROJO "%14.2f\n" RESET, comuna->satisfaccion);
        }

        comuna = comuna->siguiente;
    }
}

// permite seleccionar un bien o servicio
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

    opcion = pedir_opcion("\nSeleccione una opcion: ", 1, i - 1);

    seleccionado = lista;

    for (i = 1; i < opcion; i++) {
        seleccionado = seleccionado->siguiente;
    }

    return seleccionado;
}

// permite intercambiar bienes o servicios entre dos comunas
void menu_trueque(struct Sociedad *sociedad) {
    struct Comuna *origen;
    struct Comuna *destino;
    struct Comuna *actual;
    struct Existencia *elementoOrigen;
    struct Existencia *elementoDestino;
    struct Existencia *listaOrigen;
    struct Existencia *listaDestino;
    int tipo;
    int numero;
    int i;
    int cantidad;
    int resultado;

    mostrar_titulo("MENU DE TRUEQUE");

    printf("1. Intercambiar bienes\n");
    printf("2. Intercambiar servicios\n");
    printf("0. Continuar sin intercambiar\n\n");

    tipo = pedir_opcion("Opcion: ", 0, 2);

    if (tipo == 0) {
        printf(AMARILLO "\nSe continua sin realizar trueques.\n" RESET);
        return;
    }

    if (sociedad->cantidadComunas < 2) {
        mostrar_error("Se necesitan al menos dos comunas para intercambiar.");
        return;
    }

    printf("\n" NEGRITA "COMUNAS DISPONIBLES\n" RESET);

    actual = sociedad->comunas;
    i = 1;

    while (actual != NULL) {
        printf("  %d. %s\n", i, actual->nombre);
        actual = actual->siguiente;
        i++;
    }

    numero = pedir_opcion("\nNumero de la comuna que entrega: ",
                          1, sociedad->cantidadComunas);

    origen = sociedad->comunas;

    for (i = 1; i < numero; i++) {
        origen = origen->siguiente;
    }

    // vuelve a pedir la comuna si se selecciona la misma
    do {
        numero = pedir_opcion("Numero de la comuna que recibe: ",
                              1, sociedad->cantidadComunas);

        destino = sociedad->comunas;

        for (i = 1; i < numero; i++) {
            destino = destino->siguiente;
        }

        if (destino == origen) {
            mostrar_error("Debe seleccionar una comuna diferente.");
        }

    } while (destino == origen);

    // selecciona las listas segun el tipo de intercambio
    if (tipo == 1) {
        listaOrigen = origen->bienes;
        listaDestino = destino->bienes;
    } else {
        listaOrigen = origen->servicios;
        listaDestino = destino->servicios;
    }

    printf("\n" AZUL "Comuna que entrega: %s\n" RESET, origen->nombre);

    elementoOrigen = seleccionar_existencia(
        listaOrigen, "Seleccione lo que entrega esta comuna:");

    if (elementoOrigen == NULL) {
        return;
    }

    printf("\n" AZUL "Comuna que recibe: %s\n" RESET, destino->nombre);

    elementoDestino = seleccionar_existencia(
        listaDestino, "Seleccione lo que entrega la otra comuna:");

    if (elementoDestino == NULL) {
        return;
    }

    // vuelve a pedir la cantidad si no es valida
    do {
        printf("\nCantidad que intercambiara cada comuna: ");

        if (scanf("%d", &cantidad) != 1) {
            limpiar_entrada();
            mostrar_error("Ingrese una cantidad numerica.");
            cantidad = 0;
        } else {
            limpiar_entrada();

            if (cantidad <= 0) {
                mostrar_error("La cantidad debe ser mayor que cero.");
            }
        }

    } while (cantidad <= 0);

    // realiza el intercambio seleccionado
    if (tipo == 1) {
        resultado = realizar_trueque(
            origen, destino,
            elementoOrigen->nombre,
            elementoDestino->nombre,
            cantidad);
    } else {
        resultado = realizar_trueque_servicios(
            origen, destino,
            elementoOrigen->nombre,
            elementoDestino->nombre,
            cantidad);
    }

    if (resultado) {
        mostrar_exito("El intercambio se completo correctamente.");
    } else {
        mostrar_error("No se pudo realizar el intercambio. Revise las existencias.");
    }
}

// muestra el resumen de la simulacion
void mostrar_resumen(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;
    float totalNecesidad = 0;
    float totalSatisfaccion = 0;
    int cantidadComunas = 0;
    int poblacion = 0;

    mostrar_titulo("RESUMEN FINAL DE LA SIMULACION");

    while (comuna != NULL) {
        totalNecesidad += comuna->necesidad;
        totalSatisfaccion += comuna->satisfaccion;
        poblacion += comuna->poblacion;
        cantidadComunas++;

        comuna = comuna->siguiente;
    }

    printf("Dias simulados: %d\n", sociedad->dia);
    printf("Poblacion total: %d\n", poblacion);
    printf("Comunas registradas: %d\n", cantidadComunas);

    if (cantidadComunas > 0) {
        printf("Necesidad promedio: %.2f\n",
               totalNecesidad / cantidadComunas);

        printf("Satisfaccion promedio: %.2f\n",
               totalSatisfaccion / cantidadComunas);
    }

    printf("\n" NEGRITA "ESTADO DE CADA COMUNA\n" RESET);

    comuna = sociedad->comunas;

    while (comuna != NULL) {
        printf("\n%s\n", comuna->nombre);
        printf("  Necesidad: %.2f\n", comuna->necesidad);
        printf("  Satisfaccion: %.2f\n", comuna->satisfaccion);

        if (comuna->satisfaccion >= 70) {
            printf(VERDE "  Estado: estable\n" RESET);
        } else if (comuna->satisfaccion >= 40) {
            printf(AMARILLO "  Estado: necesita mejoras\n" RESET);
        } else {
            printf(ROJO "  Estado: necesita ayuda\n" RESET);
        }

        comuna = comuna->siguiente;
    }

    printf("\n");
}

// inicia y ejecuta la simulacion
int main(void) {
    struct Sociedad sociedad = {0};
    int dia;

    srand((unsigned int) time(NULL));

    mostrar_titulo("SIMULACION DE LA SOCIEDAD SOLARPUNK");

    printf(AMARILLO NEGRITA);
    printf("\n     AUTORIDAD SUPREMA, TENGANOS PIEDAD\n\n");
    printf(RESET);

    printf("Iniciando la sociedad...\n");

    // carga los datos iniciales desde los archivos
    if (!inicializar_sociedad(
            &sociedad,
            "Datos/personas",
            "Datos/comunas",
            "Datos/bienes",
            "Datos/servicios")) {

        mostrar_error("No se pudo inicializar la sociedad.");
        return 1;
    }

    calcular_indices_sociedad(&sociedad);

    mostrar_exito("La sociedad se inicializo correctamente.");

    mostrar_comunas(&sociedad);

    // ejecuta los dias de la simulacion
    for (dia = 1; dia <= 15; dia++) {
        sociedad.dia = dia;

        mostrar_titulo("NUEVO DIA DE SIMULACION");

        printf(NEGRITA "DIA %d DE 15\n" RESET, dia);

        // genera una emergencia cuando llega el momento
        if (sociedad.diasParaEmergencia <= 0) {
            generar_emergencia(&sociedad);
        }

        if (dia < 15) {
            menu_trueque(&sociedad);
        }

        // aplica el consumo y actualiza los indices
        aplicar_consumo_diario(&sociedad);
        calcular_indices_sociedad(&sociedad);

        printf("\n" CIAN "Estado al finalizar el dia %d:\n" RESET, dia);
        mostrar_comunas(&sociedad);
    }

    // presenta los resultados de los dias simulados
    mostrar_resumen(&sociedad);

    mostrar_exito("La simulacion ha terminado.");

    liberar_sociedad(&sociedad);

    return 0;
}
