
#include <stdio.h>
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

        // la penalizacion de las emergencias se recupera de a poco
        if (comuna->penalizacionEmergencia > 0) {
            comuna->penalizacionEmergencia -= 2;

            if (comuna->penalizacionEmergencia < 0) {
                comuna->penalizacionEmergencia = 0;
            }
        }

        // actualiza la necesidad y satisfaccion
        calcular_indices_comuna(comuna);

        comuna = comuna->siguiente;
    }

    // avanza al siguiente dia
    sociedad->dia++;

    // cuenta los dias que faltan para la proxima emergencia
    sociedad->diasParaEmergencia--;

    if (sociedad->diasParaEmergencia <= 0) {
        generar_emergencia(sociedad);

        // la proxima emergencia sale entre 3 y 7 dias despues
        sociedad->diasParaEmergencia = (rand() % 5) + 3;
    }
}
