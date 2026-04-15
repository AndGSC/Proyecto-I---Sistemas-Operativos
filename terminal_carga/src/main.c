#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "config.h"
#include "camion.h"
#include "cola.h"
#include "sync.h"
#include "log.h"
#include "scheduler.h"
#include "stats.h"

// Variable global para controlar el algoritmo actual (FIFO por defecto)
int politica_actual = POLITICA_FIFO;

/**
 * Inicializa los datos de un camión con valores aleatorios.
 * Simula la llegada de diferentes tipos de carga.
 */
void preparar_camion(Camion *c, int id) {
    c->id = id;
    // Prioridad de 1 a 5. El 5 representa productos perecederos.
    c->prioridad = rand() % 5 + 1; 
    
    // Ráfaga de CPU (tiempo de carga) aleatoria entre los límites de config.h
    c->burst_total = rand() % (BURST_MAX - BURST_MIN + 1) + BURST_MIN;
    c->burst_restante = c->burst_total;
    
    // En esta simulación simplificada, todos llegan en el tiempo t=0
    c->tiempo_llegada = 0; 
    c->tiempo_inicio = 0;
    c->tiempo_fin = 0;
    c->tiempo_espera = 0;
    c->turnaround = 0;
    c->estado = NUEVO;
}

int main(int argc, char *argv[]) {
    // Inicializar semilla para aleatorios
    srand(time(NULL));
    
    // 1. Determinar política de planificación por argumentos de línea de comandos
    // Uso: ./terminal 1 (FIFO) o ./terminal 2 (Round Robin)
    if (argc > 1) {
        int opcion = atoi(argv[1]);
        if (opcion == POLITICA_RR) {
            politica_actual = POLITICA_RR;
        }
    }

    printf("\n=== SISTEMA DE GESTIÓN DE TERMINAL DE CARGA AUTOMATIZADA ===\n");
    printf("Algoritmo activo: %s\n", (politica_actual == POLITICA_FIFO) ? "FIFO (con Prioridad)" : "Round Robin");
    printf("Muelles de carga (Recurso compartido): %d\n", MAX_MUELLES);
    printf("Quantum definido: %d unidades\n", QUANTUM);
    printf("============================================================\n\n");

    // 2. Inicializar Módulos de Soporte y Sincronización
    init_log();
    init_sync(MAX_MUELLES); // Inicializa semáforos y mutex definidos en sync.c
    
    Cola cola_listos;
    init_cola(&cola_listos);

    Camion flota[MAX_CAMIONES];
    int total_camiones = 10; // Cantidad de hilos a simular para esta prueba

    // 3. Fase de Admisión: Crear camiones e insertarlos en la cola de LISTOS
    // Se utiliza 'encolar_con_prioridad' para cumplir el requisito de perecederos.
    for (int i = 0; i < total_camiones; i++) {
        preparar_camion(&flota[i], i + 1);
        
        pthread_mutex_lock(&mutex_cola);
        encolar_con_prioridad(&cola_listos, &flota[i]);
        pthread_mutex_unlock(&mutex_cola);
        
        escribir_log("[SISTEMA] Camión %d creado (Prioridad: %d, Burst: %d)", 
                     flota[i].id, flota[i].prioridad, flota[i].burst_total);
    }

    // 4. Despacho de Hilos: Se crean los hilos POSIX
    // Cada hilo ejecutará 'rutina_camion' de forma concurrente.
    for (int i = 0; i < total_camiones; i++) {
        if (pthread_create(&flota[i].hilo, NULL, rutina_camion, &flota[i]) != 0) {
            perror("Fallo crítico: No se pudo crear el hilo del camión");
            return EXIT_FAILURE;
        }
    }

    // 5. Sincronización de Barrera: Esperar a que todos los hilos terminen
    // El programa principal se bloquea aquí hasta que el último camión complete su carga.
    for (int i = 0; i < total_camiones; i++) {
        pthread_join(flota[i].hilo, NULL);
    }

    // 6. Reporte Final y Estadísticas
    mostrar_estadisticas_camiones(flota, total_camiones);
    mostrar_resumen_estadistico(flota, total_camiones);

    // 7. Liberación de Recursos (Limpieza)
    destruir_cola(&cola_listos);
    destroy_sync();
    cerrar_log();

    printf("\nSimulación finalizada exitosamente.\n");
    printf("Consulte '%s' para ver el historial de cambios de estado.\n\n", LOG_FILE);

    return EXIT_SUCCESS;
}