#ifndef COLA_H
#define COLA_H

#include "camion.h"

/* Nodo de la cola */
typedef struct Nodo {
    Camion *camion;
    struct Nodo *sig;
} Nodo;

/* Estructura de la cola */
typedef struct {
    Nodo *frente;
    Nodo *final;
    int tamano;
} Cola;

/* Operaciones de la cola */
void init_cola(Cola *c);
int cola_vacia(Cola *c);
void insertar_camion(Cola *c, Camion *camion);
Camion *sacar_camion(Cola *c);
Camion *primer_camion(Cola *c);
void destruir_cola(Cola *c);

#endif
