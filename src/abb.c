#include "abb.h"
#include "abb_estructura_privada.h"

// ----- FUNCIONES AUXILIARES
nodo_t* nodo_crear(void* elemento) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo->elemento = elemento;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void nodo_destruir(nodo_t* nodo) {
    if (!nodo) return;
    nodo_destruir(nodo->izq);
    nodo_destruir(nodo->der);
    free(nodo);
}

void nodo_destruir_todo(nodo_t* nodo, void (*destructor)(void*)) {
    if (!nodo) return;
    nodo_destruir_todo(nodo->izq, destructor);
    nodo_destruir_todo(nodo->der, destructor);
    destructor(nodo->elemento);
    free(nodo);
}
// FIN FUNCIONES AUXILIARES

// FUNCION CREAR
abb_t* abb_crear(int (*comparador)(void*, void*)) {
    if (!comparador) return NULL;
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;
    abb->raiz = NULL;
    abb->comparador = comparador;
    abb->nodos = 0;
    return abb;
}

// FUNCION DESTRUIR
void abb_destruir(abb_t* abb) {
    if (!abb) return;
    nodo_destruir(abb->raiz);
    free(abb);
}

// FUNCION DESTRUIR TODO
void abb_destruir_todo(abb_t* abb, void (*destructor)(void*)) {
    if (!abb || !destructor) return;
    nodo_destruir_todo(abb->raiz, destructor);
    free(abb);
}

// FUNCION INSERTAR
static nodo_t* abb_insertar_rec(abb_t *abb, nodo_t *nodo, void* elemento) {
    if (nodo == NULL) {
        nodo_t *nuevo = nodo_crear(elemento);
        if (nuevo)
            abb->nodos++;
        return nuevo;
    }

    int cmp = abb->comparador(elemento, nodo->elemento);
    if (cmp > 0) {
        nodo->der = abb_insertar_rec(abb, nodo->der, elemento);
    } else {
        nodo->izq = abb_insertar_rec(abb, nodo->izq, elemento);
    }
    return nodo;
}

bool abb_insertar(abb_t *abb, void* elemento) {
    if (abb == NULL)
        return false;

    size_t nodos_iniciales = abb->nodos;
    abb->raiz = abb_insertar_rec(abb, abb->raiz, elemento);

    return abb->nodos > nodos_iniciales;
}


// FUNCION QUITAR
nodo_t* encontrar_minimo(nodo_t* nodo) {
    while (nodo->izq != NULL) {
        nodo = nodo->izq;
    }
    return nodo;
}

nodo_t* quitar_nodo(abb_t* abb, nodo_t* nodo, void* buscado, bool* fue_removido, void** encontrado) {
    if (nodo == NULL || !abb->comparador) {
        *fue_removido = false;
        return NULL;
    }

    int cmp = abb->comparador(buscado, nodo->elemento);

    if (cmp > 0) {  // buscar en el subárbol derecho
        nodo->der = quitar_nodo(abb, nodo->der, buscado, fue_removido, encontrado);
    } else if (cmp < 0) {  // buscar en el subárbol izquierdo
        nodo->izq = quitar_nodo(abb, nodo->izq, buscado, fue_removido, encontrado);
    } else {
        // Nodo encontrado
        *encontrado = nodo->elemento;  // guardar el puntero al elemento encontrado
        *fue_removido = true;

        // Caso 1: Nodo sin hijos
        if (nodo->izq == NULL && nodo->der == NULL) {
            free(nodo);
            return NULL;
        }

        // Caso 2: Nodo con un solo hijo
        if (nodo->izq == NULL) {
            nodo_t* temp = nodo->der;
            free(nodo);
            return temp;
        } else if (nodo->der == NULL) {
            nodo_t* temp = nodo->izq;
            free(nodo);
            return temp;
        }

        // Caso 3: Nodo con dos hijos
        nodo_t* min_nodo = encontrar_minimo(nodo->der);
        nodo->elemento = min_nodo->elemento;  // reemplazar valor por min de subarbol derecho
        nodo->der = quitar_nodo(abb, nodo->der, min_nodo->elemento, fue_removido, encontrado);
    }
    return nodo;
}

bool abb_quitar(abb_t* abb, void* buscado, void** encontrado) {
    if (abb->raiz == NULL) { // el árbol está vacío
        return false;
    }
    bool removido = false;
    abb->raiz = quitar_nodo(abb, abb->raiz, buscado, &removido, encontrado);

    if (removido) {
        abb->nodos--;
    } else {
        *encontrado = NULL;
    }
    return removido;
}

// FUNCION OBTENER
nodo_t* nodo_buscar(nodo_t* nodo, void* elemento, int (*comparador)(void*, void*), nodo_t** padre) {
    if (!nodo) return NULL;
    int cmp = comparador(elemento, nodo->elemento);
    if (cmp == 0) return nodo;
    *padre = nodo;
    if (cmp < 0) {
        return nodo_buscar(nodo->izq, elemento, comparador, padre);
    } else {
        return nodo_buscar(nodo->der, elemento, comparador, padre);
    }
}

void* abb_obtener(abb_t* abb, void* elemento) {
    if (!abb || !elemento) return NULL;
    nodo_t* nodo = nodo_buscar(abb->raiz, elemento, abb->comparador, NULL);
    return nodo ? nodo->elemento : NULL;
}

// FUNCION CANTIDAD
size_t abb_cantidad(abb_t* abb) {
    return abb ? abb->nodos : 0;
}

// ----- ITERADORES
// ---
// Función auxiliar para iterar en inorden
size_t nodo_iterar_inorden(nodo_t* nodo, bool (*f)(void*, void*), void* ctx) {
    if (!nodo) return 0;
    size_t contador = 0;
    contador += nodo_iterar_inorden(nodo->izq, f, ctx);
    if (!f(nodo->elemento, ctx)) return contador;
    contador++;
    contador += nodo_iterar_inorden(nodo->der, f, ctx);
    return contador;
}

// Inorder principal
size_t abb_iterar_inorden(abb_t* abb, bool (*f)(void*, void*), void* ctx) {
    if (!abb || !f) return 0;
    return nodo_iterar_inorden(abb->raiz, f, ctx);
}

// Función auxiliar para iterar en preorden
size_t nodo_iterar_preorden(nodo_t* nodo, bool (*f)(void*, void*), void* ctx) {
    if (!nodo) return 0;
    size_t contador = 0;
    if (!f(nodo->elemento, ctx)) return contador;
    contador++;
    contador += nodo_iterar_preorden(nodo->izq, f, ctx);
    contador += nodo_iterar_preorden(nodo->der, f, ctx);
    return contador;
}

// Preorden principal
size_t abb_iterar_preorden(abb_t* abb, bool (*f)(void*, void*), void* ctx) {
    if (!abb || !f) return 0;
    return nodo_iterar_preorden(abb->raiz, f, ctx);
}

// Función auxiliar para iterar en postorden
size_t nodo_iterar_postorden(nodo_t* nodo, bool (*f)(void*, void*), void* ctx) {
    if (!nodo) return 0;
    size_t contador = 0;
    contador += nodo_iterar_postorden(nodo->izq, f, ctx);
    contador += nodo_iterar_postorden(nodo->der, f, ctx);
    if (!f(nodo->elemento, ctx)) return contador;
    contador++;
    return contador;
}

// Postorden principal
size_t abb_iterar_postorden(abb_t* abb, bool (*f)(void*, void*), void* ctx) {
    if (!abb || !f) return 0;
    return nodo_iterar_postorden(abb->raiz, f, ctx);
}

// ----- VECTORIZADORES
// ---
size_t nodo_vectorizar_inorden(nodo_t* nodo, void** vector, size_t tamaño, size_t* indice) {
    if (!nodo || *indice >= tamaño) return 0;
    size_t contador = 0;
    contador += nodo_vectorizar_inorden(nodo->izq, vector, tamaño, indice);
    if (*indice < tamaño) {
        vector[*indice] = nodo->elemento;
        (*indice)++;
        contador++;
    }
    contador += nodo_vectorizar_inorden(nodo->der, vector, tamaño, indice);
    return contador;
}

// Función para vectorizar en inorden
size_t abb_vectorizar_inorden(abb_t* abb, void** vector, size_t tamaño) {
    if (!abb || !vector) return 0;
    size_t indice = 0;
    return nodo_vectorizar_inorden(abb->raiz, vector, tamaño, &indice);
}

// Función auxiliar para vectorizar en preorden
size_t nodo_vectorizar_preorden(nodo_t* nodo, void** vector, size_t tamaño, size_t* indice) {
    if (!nodo || *indice >= tamaño) return 0;
    size_t contador = 0;
    if (*indice < tamaño) {
        vector[*indice] = nodo->elemento;
        (*indice)++;
        contador++;
    }
    contador += nodo_vectorizar_preorden(nodo->izq, vector, tamaño, indice);
    contador += nodo_vectorizar_preorden(nodo->der, vector, tamaño, indice);
    return contador;
}

// Función para vectorizar en preorden
size_t abb_vectorizar_preorden(abb_t* abb, void** vector, size_t tamaño) {
    if (!abb || !vector) return 0;
    size_t indice = 0;
    return nodo_vectorizar_preorden(abb->raiz, vector, tamaño, &indice);
}

// Función auxiliar para vectorizar en postorden
size_t nodo_vectorizar_postorden(nodo_t* nodo, void** vector, size_t tamaño, size_t* indice) {
    if (!nodo || *indice >= tamaño) return 0;
    size_t contador = 0;
    contador += nodo_vectorizar_postorden(nodo->izq, vector, tamaño, indice);
    contador += nodo_vectorizar_postorden(nodo->der, vector, tamaño, indice);
    if (*indice < tamaño) {
        vector[*indice] = nodo->elemento;
        (*indice)++;
        contador++;
    }
    return contador;
}

// Función para vectorizar en postorden
size_t abb_vectorizar_postorden(abb_t* abb, void** vector, size_t tamaño) {
    if (!abb || !vector) return 0;
    size_t indice = 0;
    return nodo_vectorizar_postorden(abb->raiz, vector, tamaño, &indice);
}
