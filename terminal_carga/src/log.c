#include <stdio.h>
#include <stdarg.h> // Para va_list, va_start y va_end
#include <pthread.h> // Para pthread_mutex_t y funciones de mutex
#include "log.h"
#include "config.h"

static FILE *archivo_log = NULL;              
// Puntero estático al archivo log

static pthread_mutex_t mutex_log;             
// Mutex para proteger el acceso log

// Inicializa el sistema de log.
void init_log(void) {                         
    pthread_mutex_init(&mutex_log, NULL);     // Inicializa mutex en default
    archivo_log = fopen(LOG_FILE, "a");
  // Append - "a"
    if (archivo_log == NULL) {
        perror("Error al abrir el archivo de log");
    }
}

// Funcion para escribir mensajes en el log
void escribir_log(const char *formato, ...) { 
    va_list args; // Creación de argumentos variables
    pthread_mutex_lock(&mutex_log); 
  // Bloquea el mutex para escribir


  
    if (archivo_log != NULL) {
        va_start(args, formato); //Inicializa la lectura
        vfprintf(archivo_log, formato, args); // Escribe el mensaje en el archivo
        fprintf(archivo_log, "\n");
        fflush(archivo_log);  // Fuerza la escritura
        va_end(args);         // Finaliza el uso de argumentos variables
    }

    pthread_mutex_unlock(&mutex_log);  // Libera el mutex cuando finalice la escritura...
}

// Funcion para cerrar log
void cerrar_log(void) {                       
    pthread_mutex_lock(&mutex_log);           
  // Bloquea mutex antes

    if (archivo_log != NULL) {
        fclose(archivo_log);
        archivo_log = NULL;                   // Evita que quede apuntando a memoria invalida
    }
  // Libera el mutex:
    pthread_mutex_unlock(&mutex_log);
    pthread_mutex_destroy(&mutex_log);
}
