#ifndef SIMULACION_H
#define SIMULACION_H

#include "sociedad.h"

// aplica el consumo de un dia en todas las comunas
void aplicar_consumo_diario(struct Sociedad *sociedad);

//genera una emergencia en una comuna aleratoria
void generar_emergencia(struct Sociedad *sociedad);
#endif
