#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include "log.h"
#include "config.h"

static FILE *archivo_log = NULL;
static pthread_mutex_t mutex_log;

/**
 * Inicializa el sistema de log.
 * SOLUCIÓN: Usamos modo "w" para truncar (borrar) el archivo previo 
 * y asegurar que empezamos con un archivo de texto limpio.
 */
void init_log(void) {
    pthread_mutex_init(&mutex_log, NULL);
    
    // "w" sobreescribe el archivo. "a" lo mantenía creciendo infinitamente.
    archivo_log = fopen(LOG_FILE, "w");
    
    if (archivo_log == NULL) {
        perror("Error crítico: No se pudo crear el archivo de log");
        return;
    }

    fprintf(archivo_log, "=== INICIO DE SIMULACIÓN LOGÍSTICA ===\n");
    fprintf(archivo_log, "Archivo generado el: %ld (Segundos Epoch)\n", time(NULL));
    fprintf(archivo_log, "------------------------------------------------\n");
    fflush(archivo_log);
}

/**
 * Escribe mensajes en el log de forma segura (Thread-Safe).
 */
void escribir_log(const char *formato, ...) {
    if (archivo_log == NULL) return;

    va_list args;
    pthread_mutex_lock(&mutex_log);

    // 1. Añadir Timestamp [HH:MM:SS] para dar estructura de texto clara
    time_t ahora = time(NULL);
    struct tm *t = localtime(&ahora);
    fprintf(archivo_log, "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);

    // 2. Procesar los argumentos variables (va_list)
    va_start(args, formato);
    vfprintf(archivo_log, formato, args);
    va_end(args);

    // 3. Asegurar salto de línea y vaciado de buffer
    fprintf(archivo_log, "\n");
    fflush(archivo_log); 

    pthread_mutex_unlock(&mutex_log);
}

/**
 * Cierra el archivo y destruye el mutex de forma segura.
 */
void cerrar_log(void) {
    pthread_mutex_lock(&mutex_log);
    if (archivo_log != NULL) {
        fprintf(archivo_log, "=== FIN DE SIMULACIÓN ===\n");
        fclose(archivo_log);
        archivo_log = NULL;
    }
    pthread_mutex_unlock(&mutex_log);
    pthread_mutex_destroy(&mutex_log);
}