#ifndef ECONOMIA_H
#define ECONOMIA_H

#include "sociedad.h"

// Realiza un trueque entre dos comunas
int realizar_trueque(struct Comuna *origen,
                     struct Comuna *destino,
                     const char *bienOrigen,
                     const char *bienDestino,
                     int cantidad);
int realizar_trueque_servicios(struct Comuna *origen,
                               struct Comuna *destino,
                               const char *servicioOrigen,
                               const char *servicioDestino,
                               int cantidad);

#endif
