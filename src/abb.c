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

static nodo_t *encontrar_minimo(nodo_t *nodo)
{
	while (nodo->izq != NULL)
		nodo = nodo->izq;
	return nodo;
}

// ----- FIN FUNCIONES AUXILIARES

static nodo_t *nodo_insertar(abb_t *abb, nodo_t *nodo, void *elemento)
{
	if (nodo == NULL) {
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

static nodo_t *nodo_quitar_sin_hijos(nodo_t *nodo)
{
	if (nodo)
		free(nodo);
	return NULL;
}

static nodo_t *nodo_quitar_un_hijo(nodo_t *nodo)
{
	nodo_t *temp = (nodo->izq != NULL) ? nodo->izq : nodo->der;
	if (nodo)
		free(nodo);
	return temp;
}

static nodo_t *nodo_quitar(abb_t *abb, nodo_t *nodo, void *buscado,
			   bool *fue_removido, void **encontrado)
{
	if (nodo == NULL || !abb->comparador) {
		*fue_removido = false;
		return NULL;
	}
	int cmp = abb->comparador(buscado, nodo->elemento);

	if (cmp > 0) { // Buscar en el subárbol derecho
		nodo->der = nodo_quitar(abb, nodo->der, buscado, fue_removido,
					encontrado);
	} else if (cmp < 0) { // Buscar en el subárbol izquierdo
		nodo->izq = nodo_quitar(abb, nodo->izq, buscado, fue_removido,
					encontrado);
	} else { // Nodo encontrado
		if (encontrado)
			*encontrado = nodo->elemento;
		*fue_removido = true;

		if (nodo->izq == NULL && nodo->der == NULL)
			return nodo_quitar_sin_hijos(nodo);

		if (nodo->izq == NULL || nodo->der == NULL)
			return nodo_quitar_un_hijo(nodo);

		// Caso 3: Nodo con dos hijos
		nodo_t *min_nodo = encontrar_minimo(nodo->der);
		nodo->elemento =
			min_nodo->elemento; // Reemplazar valor por el mínimo del subárbol derecho
		nodo->der = nodo_quitar(abb, nodo->der, min_nodo->elemento,
					fue_removido, &min_nodo->elemento);
	}
	return nodo;
}

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

// ----- FUNCIONES DEL ABB -----
bool abb_insertar(abb_t *abb, void *elemento)
{
	if (abb == NULL)
		return false;

	size_t nodos_iniciales = abb->nodos;
	abb->raiz = nodo_insertar(abb, abb->raiz, elemento);

	return abb->nodos > nodos_iniciales;
}

bool abb_quitar(abb_t *abb, void *buscado, void **encontrado)
{
	if (!abb || !abb->raiz || !buscado) {
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
