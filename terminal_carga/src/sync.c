#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "sync.h"

// Definicion de recursos compartidos
sem_t sem_muelles;
pthread_mutex_t mutex_cola;
pthread_mutex_t mutex_stats;

void init_sync(int cantidad_muelles) {
    if (pthread_mutex_init(&mutex_cola, NULL) != 0) {
        perror("Error al inicializar mutex de cola");
    }
    if (pthread_mutex_init(&mutex_stats, NULL) != 0) {
        perror("Error al inicializar mutex de estadísticas");
    }
    if (sem_init(&sem_muelles, 0, cantidad_muelles) != 0) {
        perror("Error al inicializar el semáforo de muelles");
    }
}

void destroy_sync(void) {
    pthread_mutex_destroy(&mutex_cola);
    pthread_mutex_destroy(&mutex_stats);
    sem_destroy(&sem_muelles);
}
