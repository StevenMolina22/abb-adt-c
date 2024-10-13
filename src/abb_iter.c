
#include "abb.h"
#include "abb_estructura_privada.h"

static size_t nodo_iterar_inorden(nodo_t* nodo, bool (*f)(void*, void*), void* ctx) {
    if (!nodo) return 0;
    size_t contador = 0;
    contador += nodo_iterar_inorden(nodo->izq, f, ctx);
    if (!f(nodo->elemento, ctx)) return contador;
    contador++;
    contador += nodo_iterar_inorden(nodo->der, f, ctx);
    return contador;
}

static size_t nodo_iterar_preorden(nodo_t* nodo, bool (*f)(void*, void*), void* ctx) {
    if (!nodo) return 0;
    size_t contador = 0;
    if (!f(nodo->elemento, ctx)) return contador;
    contador++;
    contador += nodo_iterar_preorden(nodo->izq, f, ctx);
    contador += nodo_iterar_preorden(nodo->der, f, ctx);
    return contador;
}

static size_t nodo_iterar_postorden(nodo_t* nodo, bool (*f)(void*, void*), void* ctx) {
    if (!nodo) return 0;
    size_t contador = 0;
    contador += nodo_iterar_postorden(nodo->izq, f, ctx);
    contador += nodo_iterar_postorden(nodo->der, f, ctx);
    if (!f(nodo->elemento, ctx)) return contador;
    contador++;
    return contador;
}

size_t abb_iterar_inorden(abb_t* abb, bool (*f)(void*, void*), void* ctx) {
    if (!abb || !f) return 0;
    return nodo_iterar_inorden(abb->raiz, f, ctx);
}

size_t abb_iterar_preorden(abb_t* abb, bool (*f)(void*, void*), void* ctx) {
    if (!abb || !f) return 0;
    return nodo_iterar_preorden(abb->raiz, f, ctx);
}

size_t abb_iterar_postorden(abb_t* abb, bool (*f)(void*, void*), void* ctx) {
    if (!abb || !f) return 0;
    return nodo_iterar_postorden(abb->raiz, f, ctx);
}
