
#ifndef EMERGENCIAS_H
#define EMERGENCIAS_H

#include "sociedad.h"

// genera una emergencia en una comuna aleatoria
void generar_emergencia(struct Sociedad *sociedad);

// afecta la satisfaccion de las comunas cercanas a una emergencia
void afectar_comunas_cercanas(struct Sociedad *sociedad,
                              struct Comuna *afectada);

#endif
