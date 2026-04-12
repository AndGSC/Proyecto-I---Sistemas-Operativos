#include <pthread.h>
#include "scheduler.h"
#include "sync.h"

/* Selecciona el siguiente camión por FIFO. Además, extrae el primer camion disponible de la cola.*/
Camion *planificar_fifo(Cola *cola) {
    Camion *camion;   // Variable para guardar el camion extraido de la cola

    if (cola == NULL) {return NULL;}

    pthread_mutex_lock(&mutex_cola);   // Bloquea la cola para acceso seguro
    camion = sacar_camion(cola);       // Saca el primer camion de la cola
    pthread_mutex_unlock(&mutex_cola); // Libera la cola
    return camion;   // Retorna el camion seleccionado
}

/*  Selecciona el siguiente camión según Round Robin. Asimismo, se extrae el primer camión disponible de la cola. */
Camion *planificar_round_robin(Cola *cola) {
    Camion *camion;   // Variable para guardar el camion extraido de la cola

    if (cola == NULL) { return NULL; }

    //Análogo a FIFO:
    pthread_mutex_lock(&mutex_cola);
    camion = sacar_camion(cola);
    pthread_mutex_unlock(&mutex_cola);
    return camion;
}

/* Reingresa un camión a la cola por Round Robin. Esto es si no termina su ráfaga por el quantum.*/
void reinsertar_camion_rr(Cola *cola, Camion *camion) {
    if (cola == NULL || camion == NULL) {return;}

    pthread_mutex_lock(&mutex_cola);
    insertar_camion(cola, camion);      // Inserta nuevamente el camion al final de la cola
    pthread_mutex_unlock(&mutex_cola);
}
