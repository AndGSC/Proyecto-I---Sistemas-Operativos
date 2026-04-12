#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "cola.h"
#include "camion.h"

// Tipos de planificacion con FIFO y Round Robin
typedef enum {
    FIFO = 1,
    ROUND_ROBIN = 2
} PoliticaPlanificacion;

// Obtiene camión por FIFO
Camion *planificar_fifo(Cola *cola);

// Obtiene camión por Round Robin
Camion *planificar_round_robin(Cola *cola);

// Reingresa camión a la cola por Round Robin
void reinsertar_camion_rr(Cola *cola, Camion *camion);

#endif
