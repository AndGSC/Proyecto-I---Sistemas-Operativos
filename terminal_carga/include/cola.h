#ifndef COLA_H
#define COLA_H

#include "camion.h"

/* Nodo de la cola */
typedef struct Nodo {
    Camion *camion;
    struct Nodo *sig;
} Nodo;

/* struct de la cola */
typedef struct {
    Nodo *frente;
    Nodo *final;
    int tamano;
} Cola;

/* 
Operaciones de la cola
Nota: esta cola no maneja sincronizacion interna.
Si varios hilos acceden a ella, debe protegerse externamente con mutex.
*/
void init_cola(Cola *c);
int cola_vacia(Cola *c);
void insertar_camion(Cola *c, Camion *camion);
Camion *sacar_camion(Cola *c);
Camion *primer_camion(Cola *c);

/* Libera solo los nodos de la cola, no los camiones */
void destruir_cola(Cola *c);

#endif
