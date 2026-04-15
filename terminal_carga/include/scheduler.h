#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cola.h"
#include "camion.h"

// Nueva función para manejar la prioridad de productos perecederos (Prioridad 5)
void encolar_con_prioridad(Cola *cola, Camion *camion);

Camion *planificar_fifo(Cola *cola);
Camion *planificar_round_robin(Cola *cola);
void reinsertar_camion_rr(Cola *cola, Camion *camion);

#endif