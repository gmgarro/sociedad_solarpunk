
#include <stdlib.h>
#include "consumo.h"
#include "emergencias.h"

// reduce una cantidad aleatoria de una existencia
static void consumir_existencias(struct Existencia *existencias) {
    struct Existencia *actual = existencias;

    while (actual != NULL) {
        int consumo = (rand() % 5) + 1;

        if (actual->cantidad < consumo) {
            actual->cantidad = 0;
        } else {
            actual->cantidad -= consumo;
        }

        actual = actual->siguiente;
    }
}

// aplica el consumo de un dia a todas las comunas
void aplicar_consumo_diario(struct Sociedad *sociedad) {
    struct Comuna *comuna = sociedad->comunas;

    // recorre todas las comunas
    while (comuna != NULL) {
        consumir_existencias(comuna->bienes);
        consumir_existencias(comuna->servicios);

        // actualiza la necesidad y satisfaccion
        calcular_indices_comuna(comuna);

        comuna = comuna->siguiente;
    }

    // avanza al siguiente dia
    sociedad->dia++;

    // genera una emergencia cada cinco dias
    if (sociedad->dia % 5 == 0) {
        generar_emergencia(sociedad);
    }
}
