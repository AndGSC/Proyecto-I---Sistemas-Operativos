#include <stdio.h>
#include "stats.h"
#include "config.h"
#include "sync.h"

void calcular_tiempos_camion(Camion *camion) {
    if (camion == NULL) return;

    // Turnaround = Tiempo total en el sistema (Fin - Llegada)
    // Usamos una simulación de tiempo basada en el burst para que los datos sean lógicos
    // ya que el tiempo real de CPU es demasiado rápido.
    if (camion->tiempo_fin < camion->tiempo_inicio) {
        camion->tiempo_fin = camion->tiempo_inicio + (camion->burst_total);
    }
    
    camion->turnaround = camion->tiempo_fin - camion->tiempo_llegada;
    camion->tiempo_espera = camion->turnaround - camion->burst_total;

    if (camion->tiempo_espera < 0) camion->tiempo_espera = 0;
}

void mostrar_estadisticas_camiones(Camion camiones[], int cantidad) {
    printf("\n==============================================================================\n");
    printf("                  DETALLE DE TIEMPOS POR CAMIÓN (HILOS)                       \n");
    printf("==============================================================================\n");
    printf("| ID | Prio | Burst | Llegada | Inicio |  Fin  | Espera | Turnaround |\n");
    printf("------------------------------------------------------------------------------\n");

    for (int i = 0; i < cantidad; i++) {
        printf("| %2d |  %2d  |  %3d  |   %3d   |  %4d  | %4d  |  %4d  |    %4d    |\n",
               camiones[i].id, camiones[i].prioridad, camiones[i].burst_total,
               camiones[i].tiempo_llegada, camiones[i].tiempo_inicio,
               camiones[i].tiempo_fin, camiones[i].tiempo_espera, camiones[i].turnaround);
    }
}

void mostrar_resumen_estadistico(Camion camiones[], int cantidad) {
    float suma_espera = 0;
    float suma_turnaround = 0;

    for (int i = 0; i < cantidad; i++) {
        suma_espera += camiones[i].tiempo_espera;
        suma_turnaround += camiones[i].turnaround;
    }

    printf("\n PROMEDIOS GENERALES:\n");
    printf(" > Tiempo de Espera Promedio:   %.2f unidades\n", suma_espera / cantidad);
    printf(" > Tiempo de Turnaround Promedio: %.2f unidades\n", suma_turnaround / cantidad);
    printf("==============================================================================\n");
}