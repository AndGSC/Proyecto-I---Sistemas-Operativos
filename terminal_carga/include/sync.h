#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include <semaphore.h>

// Semáforo para controlar acceso a los muelles:
extern sem_t sem_muelles;
// Mutex para proteger la cola de camiones:
extern pthread_mutex_t mutex_cola;
// Mutex para proteger estadisticas compartidas:
extern pthread_mutex_t mutex_stats;
// Inicializa la sincronización:
void init_sync(int cantidad_muelles);
// Libera los recursos de sincronización:
void destroy_sync(void);

#endif
