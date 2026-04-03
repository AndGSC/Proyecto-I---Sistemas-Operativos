#include <stdlib.h>
#include "cola.h"

void init_cola(Cola *c) {
    c->frente = NULL;
    c->final = NULL;
    c->tamano = 0;
}

int cola_vacia(Cola *c) {
    return (c->frente == NULL);
}

void insertar_camion(Cola *c, Camion *camion) {
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        return;
    }

    nuevo->camion = camion;
    nuevo->sig = NULL;

    if (cola_vacia(c)) {
        c->frente = nuevo;
        c->final = nuevo;
    } else {
        c->final->sig = nuevo;
        c->final = nuevo;
    }

    c->tamano++;
}

Camion *sacar_camion(Cola *c) {
    Nodo *temp;
    Camion *camion;

    if (cola_vacia(c)) {
        return NULL;
    }

    temp = c->frente;
    camion = temp->camion;
    c->frente = c->frente->sig;

    if (c->frente == NULL) {
        c->final = NULL;
    }

    free(temp);
    c->tamano--;

    return camion;
}

Camion *primer_camion(Cola *c) {
    if (cola_vacia(c)) {
        return NULL;
    }

    return c->frente->camion;
}

void destruir_cola(Cola *c) {
    Nodo *actual = c->frente;
    Nodo *siguiente;

    while (actual != NULL) {
        siguiente = actual->sig;
        free(actual);
        actual = siguiente;
    }

    c->frente = NULL;
    c->final = NULL;
    c->tamano = 0;
}
