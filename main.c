
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct Persona *buscar_persona(struct Sociedad *sociedad, int numero);

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
int menu_trueque(struct Sociedad *sociedad) {
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
        return 0;
    }

    if (sociedad->cantidadComunas < 2) {
        mostrar_error("Se necesitan al menos dos comunas para intercambiar.");
        return 0;
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

    if (!origen->administrada) {
      mostrar_error("Solo puedes iniciar trueques desde tus comunas administradas.");
    return 1;
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
        return 1;
    }

    printf("\n" AZUL "Comuna que recibe: %s\n" RESET, destino->nombre);

    elementoDestino = seleccionar_existencia(
        listaDestino, "Seleccione lo que entrega la otra comuna:");

    if (elementoDestino == NULL) {
        return 1;
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
    return 1;
}

// muestra las comunas y devuelve la que elija el usuario
struct Comuna *seleccionar_comuna(struct Sociedad *sociedad,
                                  const char *mensaje) {
    struct Comuna *actual = sociedad->comunas;
    int numero;
    int i = 1;

    printf("\n" NEGRITA "COMUNAS DISPONIBLES\n" RESET);

    while (actual != NULL) {
        printf("  %d. %s\n", i, actual->nombre);
        actual = actual->siguiente;
        i++;
    }

    numero = pedir_opcion(mensaje, 1, sociedad->cantidadComunas);

    actual = sociedad->comunas;

    for (i = 1; i < numero; i++) {
        actual = actual->siguiente;
    }

    return actual;
}

// muestra las personas de una comuna con su numero y sus servicios
void mostrar_personas_comuna(struct Sociedad *sociedad,
                             struct Comuna *comuna) {
    struct Persona *p = sociedad->personas;
    int numero = 1;

    printf("\n" NEGRITA "PERSONAS DE %s\n" RESET, comuna->nombre);

    while (p != NULL) {
        if (p->comunaActual == comuna) {
            struct ServicioPersona *s = p->servicios;

            printf("  %3d. %-28s", numero, p->nombre);

            if (s == NULL) {
                printf(" sin servicios");
            }

            while (s != NULL) {
                printf(" [%s]", s->nombre);
                s = s->siguiente;
            }

            printf("  (dados: %d, recibidos: %d)\n",
                   p->regalosDados, p->regalosRecibidos);
        }

        p = p->siguiente;
        numero++;
    }
}

// pide dos personas por su numero y realiza el regalo
void menu_regalo(struct Sociedad *sociedad) {
    struct Persona *da;
    struct Persona *recibe;
    struct Existencia *bien;
    int numero;

    numero = pedir_opcion("\nNumero de la persona que regala: ",
                          1, sociedad->poblacionTotal);
    da = buscar_persona(sociedad, numero);

    if (!da->comunaActual->administrada) {
        mostrar_error("Solo puedes gestionar regalos desde tus comunas administradas.");
    return;
}

    numero = pedir_opcion("Numero de la persona que recibe: ",
                          1, sociedad->poblacionTotal);
    recibe = buscar_persona(sociedad, numero);

    printf("\n" AZUL "%s le regala a %s\n" RESET, da->nombre, recibe->nombre);

    bien = seleccionar_existencia(da->comunaActual->bienes,
                                  "Seleccione el bien que se regala:");

    if (bien == NULL) {
        return;
    }

    if (realizar_regalo(da, recibe, bien->nombre)) {
        calcular_indices_sociedad(sociedad);
        mostrar_exito("El regalo se realizo correctamente.");
    } else {
        mostrar_error("No se pudo realizar el regalo.");
    }
}

// una persona busca un texto en la biblioteca de su comuna y lee un libro
void menu_lectura(struct Sociedad *sociedad) {
    struct Persona *p;
    struct NodoNombre *servicio;
    Biblioteca *bib;
    const char *libro;
    char texto[200];
    int cantidad;
    int limite;
    int numero;
    int i;

    numero = pedir_opcion("\nNumero de la persona que lee: ",
                          1, sociedad->poblacionTotal);
    p = buscar_persona(sociedad, numero);

    // <--- CAMBIO 1: Validar que sea de una de tus 3 comunas administradas
    if (!p->comunaActual->administrada) {
        mostrar_error("Esa persona pertenece a una comuna autonoma (no administrada por ti).");
        return;
    }

    bib = p->comunaActual->biblioteca;

    printf("Texto a buscar: ");

    if (fgets(texto, sizeof(texto), stdin) == NULL) {
        return;
    }

    texto[strcspn(texto, "\n")] = '\0';

    if (texto[0] == '\0') {
        mostrar_error("El texto no puede estar vacio.");
        return;
    }

    //Cambiar 'false' por 'true' para mostrar las rotaciones Splay en vivo
    cantidad = biblioteca_buscar_texto(bib, texto, true);

    // <--- CAMBIO 3: Si no hay resultados locales, encolar en la Red Interbibliotecaria (Extra 10%)
    if (cantidad <= 0) {
        mostrar_error("No se encontraron libros localmente. Encolando solicitud en la Red Interbibliotecaria...");
        encolar_solicitud_biblioteca(sociedad, p->comunaActual, p->nombre, texto);
        return;
    }

    // muestra como maximo 10 resultados para poder elegir
    limite = cantidad < 10 ? cantidad : 10;

    printf("\n" NEGRITA "Libros encontrados (%d):\n" RESET, cantidad);

    for (i = 0; i < limite; i++) {
        printf("  %d. %s\n", i + 1, biblioteca_obtener_resultado_texto(bib, i));
    }

    numero = pedir_opcion("\nLibro que lee (0 para ninguno): ", 0, limite);

    if (numero == 0) {
        return;
    }

    libro = biblioteca_obtener_resultado_texto(bib, numero - 1);

    if (libro == NULL) {
        return;
    }

    if (registrar_libro_leido(p, libro)) {
        printf("%s ha leido %d libro(s) distinto(s).\n",
               p->nombre, contar_libros_leidos(p));
    } else {
        mostrar_error("Ya habia leido ese libro.");
    }

    if (contar_libros_leidos(p) < 3) {
        return;
    }

    printf("\n" NEGRITA "Ya puede aprender un servicio:\n" RESET);

    servicio = sociedad->catalogoServicios;

    for (i = 1; i <= sociedad->cantidadServiciosActivos; i++) {
        printf("  %d. %s\n", i, servicio->nombre);
        servicio = servicio->siguiente;
    }

    numero = pedir_opcion("\nServicio que aprende: ",
                          1, sociedad->cantidadServiciosActivos);

    servicio = sociedad->catalogoServicios;

    for (i = 1; i < numero; i++) {
        servicio = servicio->siguiente;
    }

    if (aprender_servicio_leyendo(p, servicio->nombre)) {
        mostrar_exito("La persona aprendio el servicio.");
    } else {
        mostrar_error("No pudo aprenderlo (quizas ya lo sabia).");
    }
}


// menu de la biblioteca de una comuna
void menu_biblioteca(struct Sociedad *sociedad) {
    struct Comuna *comuna;
    char nombre[200];
    int opcion;

    do {
        mostrar_titulo("MENU DE BIBLIOTECA");

        printf("1. Ver catalogo de libros (primeros 20)\n");
        printf("2. Buscar un libro por nombre exacto\n");
        printf("3. Ver el estado de la cache de una comuna\n");
        printf("0. Continuar\n\n");

        opcion = pedir_opcion("Opcion: ", 0, 3);

        if (opcion == 0) {
            continue;
        }

        comuna = seleccionar_comuna(sociedad, "\nNumero de la comuna: ");

        if (opcion == 1) {
            biblioteca_listar_catalogo(comuna->biblioteca, 20);
        } else if (opcion == 2) {
            printf("Nombre exacto del archivo: ");

            if (fgets(nombre, sizeof(nombre), stdin) != NULL) {
                nombre[strcspn(nombre, "\n")] = '\0';

                if (!biblioteca_buscar_libro(comuna->biblioteca, nombre, 3)) {
                    mostrar_error("No existe un libro con ese nombre.");
                }
            }
        } else {
            biblioteca_mostrar_estado_cache(comuna->biblioteca);
        }
    } while (opcion != 0);
}

// se repite hasta que el usuario elija continuar
void menu_personas(struct Sociedad *sociedad) {
    int opcion;

    do {
        mostrar_titulo("MENU DE PERSONAS");

        printf("1. Ver las personas de una comuna\n");
        printf("2. Regalar un bien\n");
        printf("3. Leer un libro \n");
        printf("0. Continuar\n\n");

        opcion = pedir_opcion("Opcion: ", 0, 3);

        if (opcion == 1) {
            mostrar_personas_comuna(sociedad,
                seleccionar_comuna(sociedad, "\nNumero de la comuna: "));
        } else if (opcion == 2) {
            menu_regalo(sociedad);
        } else if (opcion == 3) {
            menu_lectura(sociedad);
        }
    } while (opcion != 0);
}

// busca una persona por su numero en la lista (empieza en 1)
struct Persona *buscar_persona(struct Sociedad *sociedad, int numero) {
    struct Persona *p = sociedad->personas;
    int i;

    for (i = 1; i < numero && p != NULL; i++) {
        p = p->siguiente;
    }

    return p;
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

    printf("Dias simulados: %d\n", sociedad->dia-1);
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
		printf("  Tipo: %s\n", comuna->administrada ? "Administrada" : "Autonoma");
        if (comuna->satisfaccion >= 70) {
            printf(VERDE "  Estado: estable\n" RESET);
        } else if (comuna->satisfaccion >= 40) {
            printf(AMARILLO "  Estado: necesita mejoras\n" RESET);
        } else {
            printf(ROJO "  Estado: necesita ayuda\n" RESET);
        }
        
        comuna = comuna->siguiente;
    }
	if (sociedad->cantidadComunas < 3) {
		mostrar_error("RESULTADO FINAL: FRACASO, quedaron menos de 3 comunas.");
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
	
	// pide la ruta del corpus (la define el profe el dia de la revision)
    char rutaCorpus[300];

    limpiar_entrada();   // quita el salto de linea que dejo el ultimo scanf
    printf("Ruta de la carpeta del corpus: ");

    if (fgets(rutaCorpus, sizeof(rutaCorpus), stdin) == NULL) {
        liberar_sociedad(&sociedad);
        return 1;
    }

    rutaCorpus[strcspn(rutaCorpus, "\n")] = '\0';

    if (!crear_bibliotecas(&sociedad, rutaCorpus)) {
        mostrar_error("No se pudo crear la biblioteca. Revise la ruta del corpus.");
        liberar_sociedad(&sociedad);
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
				menu_personas(&sociedad);
				menu_biblioteca(&sociedad);

				while (menu_trueque(&sociedad)) {
            }
        }

        procesar_red_interbibliotecaria(&sociedad);
        simular_comunas_autonomas(&sociedad);
        // aplica el consumo y actualiza los indices
        aplicar_consumo_diario(&sociedad);
        calcular_indices_sociedad(&sociedad);
		
		int comunasAntes = sociedad.cantidadComunas;

		revisar_disoluciones(&sociedad);
		calcular_indices_sociedad(&sociedad);

		if (comunasAntes >= 3 && sociedad.cantidadComunas < 3) {
			mostrar_error("FRACASO: quedan menos de 3 comunas.");
		}
		
        printf("\n" CIAN "Estado al finalizar el dia %d:\n" RESET, dia);
        mostrar_comunas(&sociedad);
    }

    // presenta los resultados de los dias simulados
    mostrar_resumen(&sociedad);

    mostrar_exito("La simulacion ha terminado.");

    liberar_sociedad(&sociedad);

    return 0;
}
