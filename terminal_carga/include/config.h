#ifndef CONFIG_H
#define CONFIG_H

/* Cantidad maxima de camiones que se van a simular */
#define MAX_CAMIONES 100

/* Cantidad de muelles disponibles en la terminal */
#define MAX_MUELLES 3

/* Quantum para Round Robin */
#define QUANTUM 2
#define LOG_FILE "logs/terminal.log"

/* Tiempo minimo y maximo de trabajo de un camion */
#define BURST_MIN 2
#define BURST_MAX 10

/* Tiempo de espera artificial entre acciones (microsegundos) */
#define SLEEP_UNIDAD 1000000

#define POLITICA_FIFO 1
#define POLITICA_RR   2


/* Cambiar a 1 si quieres imprimir mas detalles en consola */
#define DEBUG 1

#endif
