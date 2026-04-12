#ifndef LOG_H
#define LOG_H

/* Inicialización del log */
void init_log(void);

/* Método para escritura en log */
void escribir_log(const char *formato, ...);

/* Función para cerrar el log */
void cerrar_log(void);

#endif
