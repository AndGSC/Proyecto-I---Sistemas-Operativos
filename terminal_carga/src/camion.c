#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "camion.h"
#include "config.h"
#include "sync.h"
#include "log.h"
#include "stats.h"

/**
 * DEFINICIÓN: Convierte el enum EstadoCamion a una cadena de texto.
 * Esto corrige el error de "undefined reference".
 */
const char *estado_a_texto(EstadoCamion estado) {
    switch (estado) {
        case NUEVO:      return "NUEVO";
        case LISTO:      return "LISTO";
        case EJECUCION:  return "EJECUCION";
        case BLOQUEADO:  return "BLOQUEADO";
        case TERMINADO:  return "TERMINADO";
        default:         return "DESCONOCIDO";
    }
}

/**
 * Rutina principal de cada hilo Camion.
 */
void *rutina_camion(void *arg) {
    Camion *camion = (Camion *)arg;
    
    // Simulación: Todos los camiones "nacen" en el tiempo 0 del sistema
    camion->tiempo_llegada = 0;
    camion->estado = LISTO;

    escribir_log("[ID: %d] Estado: %s - Llegó a la terminal (Carga: %d)", 
                 camion->id, estado_a_texto(camion->estado), camion->burst_total);

    while (camion->burst_restante > 0) {
        
        // 1. Intentar entrar a un muelle (Sincronización con Semáforo)
        camion->estado = BLOQUEADO;
        escribir_log("[ID: %d] Estado: %s - Esperando muelle...", 
                     camion->id, estado_a_texto(camion->estado));
        
        sem_wait(&sem_muelles); // DOWN: Si no hay muelles, el hilo se duerme aquí.

        // 2. Entrada al muelle (Sección Crítica)
        camion->estado = EJECUCION;
        
        // Registrar tiempo de inicio real si es la primera vez que entra
        if (camion->burst_restante == camion->burst_total) {
            camion->tiempo_inicio = (int)time(NULL) % 1000; 
        }

        escribir_log("[ID: %d] Estado: %s - En muelle. Carga restante: %d", 
                     camion->id, estado_a_texto(camion->estado), camion->burst_restante);

        // Calcular cuánto tiempo va a trabajar en este turno
        int tiempo_trabajo = (camion->burst_restante > QUANTUM) ? QUANTUM : camion->burst_restante;
        
        // Simulación del paso del tiempo (Representa el Burst de CPU)
        // Usamos usleep para que la simulación sea fluida pero los tiempos cambien
        usleep(tiempo_trabajo * 200000); 
        
        camion->burst_restante -= tiempo_trabajo;

        // 3. Salida del muelle
        escribir_log("[ID: %d] Saliendo del muelle. Pendiente: %d", 
                     camion->id, camion->burst_restante);
        
        sem_post(&sem_muelles); // UP: Despierta a otro hilo esperando.

        // 4. Lógica de reingreso para Round Robin
        if (camion->burst_restante > 0) {
            camion->estado = LISTO;
            escribir_log("[ID: %d] Vuelve a la cola (Preemción).", camion->id);
            // Pequeño delay para ceder el turno a otros hilos (yield)
            usleep(10000); 
        }
    }

    // 5. Finalización y cálculo de estadísticas
    camion->estado = TERMINADO;
    camion->tiempo_fin = (int)time(NULL) % 1000;
    
    // PROTECCIÓN: Solo un hilo a la vez puede actualizar las estadísticas compartidas
    pthread_mutex_lock(&mutex_stats);
    calcular_tiempos_camion(camion);
    pthread_mutex_unlock(&mutex_stats);

    escribir_log("[ID: %d] Estado: %s - Simulación terminada.", 
                 camion->id, estado_a_texto(camion->estado));

    return NULL;
}