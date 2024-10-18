#include "abb.h"
#include "abb_estructura_privada.h"

// ----- FUNCIONES AUXILIARES -----
nodo_t *nodo_crear(void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;
	nodo->elemento = elemento;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

static nodo_t *encontrar_maximo(nodo_t *nodo)
{
	while (nodo->der)
		nodo = nodo->der;
	return nodo;
}

// ----- FIN FUNCIONES AUXILIARES
static nodo_t *nodo_buscar(nodo_t *nodo, void *elemento,
			   int (*comparador)(void *, void *), nodo_t **padre)
{
	if (!nodo)
		return NULL;
	if (padre)
		*padre = nodo;
	if (!comparador)
		return NULL;

	int cmp = comparador(elemento, nodo->elemento);
	if (cmp == 0)
		return nodo;
	if (cmp < 0) {
		return nodo_buscar(nodo->izq, elemento, comparador, padre);
	} else {
		return nodo_buscar(nodo->der, elemento, comparador, padre);
	}
}
static nodo_t *nodo_insertar(abb_t *abb, nodo_t *nodo, void *elemento)
{
	if (!nodo) {
		nodo_t *nuevo = nodo_crear(elemento);
		if (nuevo)
			abb->nodos++;
		return nuevo;
	}

	if (!abb->comparador)
		return NULL;
	int cmp = abb->comparador(elemento, nodo->elemento);
	if (cmp > 0) {
		nodo->der = nodo_insertar(abb, nodo->der, elemento);
	} else {
		nodo->izq = nodo_insertar(abb, nodo->izq, elemento);
	}
	return nodo;
}

static nodo_t *nodo_quitar_un_hijo(nodo_t *nodo)
{
	// Funcion asume que nodo existe y tiene un solo hijo
	nodo_t *temp = (nodo->izq) ? nodo->izq : nodo->der;
	free(nodo);
	return temp;
}

static nodo_t *nodo_quitar(abb_t *abb, nodo_t *nodo, void *buscado,
			   bool *fue_removido, void **encontrado)
{
	if (!nodo || !abb->comparador) {
		*fue_removido = false;
		return NULL;
	}
	int cmp = abb->comparador(buscado, nodo->elemento);
	if (cmp > 0) { // Buscar en subarbol derecho
		nodo->der = nodo_quitar(abb, nodo->der, buscado, fue_removido,
					encontrado);
	} else if (cmp < 0) { // Buscar en subarbol izquierdo
		nodo->izq = nodo_quitar(abb, nodo->izq, buscado, fue_removido,
					encontrado);
	} else { // Nodo encontrado
		if (encontrado)
			*encontrado = nodo->elemento;
		*fue_removido = true;

		if (!nodo->izq && !nodo->der) {
			free(nodo);
			return NULL;
		}

		if (!nodo->izq || !nodo->der)
			return nodo_quitar_un_hijo(nodo);

		// Caso: Nodo con dos hijos
		nodo_t *max_nodo = encontrar_maximo(nodo->izq);
		void *temp = nodo->elemento;
		nodo->elemento = max_nodo->elemento;
		max_nodo->elemento = temp;
		nodo->izq = nodo_quitar(abb, nodo->izq, max_nodo->elemento,
					fue_removido, &max_nodo->elemento);
	}
	return nodo;
}

// ----- FUNCIONES DEL ABB -----
bool abb_insertar(abb_t *abb, void *elemento)
{
	if (!abb)
		return false;

	size_t nodos_iniciales = abb->nodos;
	abb->raiz = nodo_insertar(abb, abb->raiz, elemento);

	return abb->nodos > nodos_iniciales;
}

bool abb_quitar(abb_t *abb, void *buscado, void **encontrado)
{
	if (!abb) {
		return false;
	}
	bool removido = false;
	abb->raiz = nodo_quitar(abb, abb->raiz, buscado, &removido, encontrado);
	if (removido) {
		abb->nodos--;
	}
	return removido;
}

void *abb_obtener(abb_t *abb, void *elemento)
{
	if (!abb || !elemento)
		return NULL;
	nodo_t *nodo = nodo_buscar(abb->raiz, elemento, abb->comparador, NULL);
	return nodo ? nodo->elemento : NULL;
}

size_t abb_cantidad(abb_t *abb)
{
	return abb ? abb->nodos : 0;
}
