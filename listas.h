
#ifndef LISTAS_H
#define LISTAS_H

// guarda un nombre y apunta al siguiente nodo
typedef struct Nodo {
    char *nombre;
    struct Nodo *siguiente;
} Nodo;

// guarda el inicio, el final y la cantidad de nodos de la lista
typedef struct {
    Nodo *inicio;
    Nodo *final;
    int cantidad;
} Lista;

// deja la lista vacía para poder empezar a usarla
void inicializar_lista(Lista *lista);

// agrega un nombre al final de la lista
int insertar_final(Lista *lista, const char *nombre);

// recorre la lista y muestra todos los nombres
void imprimir_lista(const Lista *lista);

// libera la memoria que se usó para guardar los nodos y sus nombres
void liberar_lista(Lista *lista);

#endif
