#ifndef STATS_H
#define STATS_H
#include "camion.h"

/* Calcula tiempo de espera y turnaround de un camión */
void calcular_tiempos_camion(Camion *camion);

/* Muestra las estadisticas individuales de todos los camiones */
void mostrar_estadisticas_camiones(Camion camiones[], int cantidad);

/* Muestra los promedios generales de espera y turnaround */
void mostrar_resumen_estadistico(Camion camiones[], int cantidad);

#endif
