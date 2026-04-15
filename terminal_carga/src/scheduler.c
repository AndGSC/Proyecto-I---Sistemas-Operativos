#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "sync.h"

/**
 * Inserta un camión en la cola manteniendo el orden de prioridad.
 * Prioridad 5 = Perecederos (van primero).
 */
void encolar_con_prioridad(Cola *cola, Camion *camion) {
    if (cola == NULL || camion == NULL) return;

    // Nodo nuevo
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->camion = camion;
    nuevo->sig = NULL;

    // Caso A: La cola está vacía o el nuevo tiene más prioridad que el primero
    if (cola->frente == NULL || camion->prioridad > cola->frente->camion->prioridad) {
        nuevo->sig = cola->frente;
        cola->frente = nuevo;
        if (cola->final == NULL) cola->final = nuevo;
    } 
    else {
        // Caso B: Buscar posición (después de los que tengan prioridad igual o mayor)
        Nodo *actual = cola->frente;
        while (actual->sig != NULL && actual->sig->camion->prioridad >= camion->prioridad) {
            actual = actual->sig;
        }
        nuevo->sig = actual->sig;
        actual->sig = nuevo;
        if (nuevo->sig == NULL) cola->final = nuevo;
    }
    cola->tamano++;
}

Camion *planificar_fifo(Cola *cola) {
    if (cola == NULL) return NULL;
    pthread_mutex_lock(&mutex_cola);
    Camion *camion = sacar_camion(cola);
    pthread_mutex_unlock(&mutex_cola);
    return camion;
}

Camion *planificar_round_robin(Cola *cola) {
    return planificar_fifo(cola);
}

void reinsertar_camion_rr(Cola *cola, Camion *camion) {
    if (cola == NULL || camion == NULL) return;
    pthread_mutex_lock(&mutex_cola);
    // Al reinsertar, mantenemos la prioridad
    encolar_con_prioridad(cola, camion);
    pthread_mutex_unlock(&mutex_cola);
}