#ifndef CAMION_H
#define CAMION_H

#include <pthread.h>

typedef enum {
    NUEVO = 0,
    LISTO,
    EJECUCION,
    BLOQUEADO,
    TERMINADO
} EstadoCamion;

typedef struct {
    int id;
    int prioridad;

    /* Tiempo total requerido y tiempo restante */
    int burst_total;
    int burst_restante;

    /* Tiempos de simulacion */
    int tiempo_llegada;
    int tiempo_inicio;
    int tiempo_fin;
    int tiempo_espera;
    int turnaround;

    /* Estado actual del camion */
    EstadoCamion estado;

    /* Hilo asociado al camion */
    pthread_t hilo;
} Camion;

/* Rutina principal del hilo camion */
void *rutina_camion(void *arg);

/* Para convertir estado a texto */
const char *estado_a_texto(EstadoCamion estado);
