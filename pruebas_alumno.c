#include "pa2m.h"
#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----- FUNCIONES AUXILIARES
int comparador_int(void *a, void *b)
{
	return (*(int *)a - *(int *)b);
}

void destructor_int(void *elemento)
{
	free(elemento);
}

bool imprimir_elemento(void *elemento, void *ctx)
{
	printf("%d ", *(int *)elemento);
	return true;
}

bool contar_elemento(void *elemento, void *contador)
{
	(*(int *)contador)++;
	return true;
}

bool sumar_elementos(void* elemento, void* suma) {
    *(int*)suma += *(int*)elemento;
    return true;
}

// ----- PRUEBAS CREAR
void prueba_crear_abb()
{
    abb_t *abb = abb_crear(comparador_int);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un ABB");
	abb_destruir(abb);
}

// ----- PRUEBAS DESTRUIR
void destruir_abb_vacio_no_rompe()
{
    abb_t* abb = abb_crear(comparador_int);
    abb_destruir(abb);
    pa2m_afirmar(true, "Destruir un ABB vacío no produce errores");
}

void destruir_abb_con_elementos_no_rompe()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 20;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);

    abb_destruir(abb);
    pa2m_afirmar(true, "Destruir un ABB con elementos no produce errores");
}

void destruir_abb_con_elementos_y_funcion_destructora()
{
    abb_t* abb = abb_crear(comparador_int);
    int* elemento1 = malloc(sizeof(int));
    int* elemento2 = malloc(sizeof(int));
    *elemento1 = 10;
    *elemento2 = 20;

    abb_insertar(abb, elemento1);
    abb_insertar(abb, elemento2);

    abb_destruir_todo(abb, destructor_int);
    pa2m_afirmar(true, "Destruir un ABB con función destructora libera la memoria de los elementos");
}

// ----- INSERTAR
void insertar_elementos_devuelve_cantidad_correcta()
{
	abb_t *abb = abb_crear(comparador_int);
	int *elemento1 = malloc(sizeof(int));
	int *elemento2 = malloc(sizeof(int));
	int *elemento3 = malloc(sizeof(int));
	*elemento1 = 10, *elemento2 = 20, *elemento3 = 5;

	pa2m_afirmar(abb_insertar(abb, elemento1),
		     "Se puede insertar el elemento 10");
	pa2m_afirmar(abb_insertar(abb, elemento2),
		     "Se puede insertar el elemento 20");
	pa2m_afirmar(abb_insertar(abb, elemento3),
		     "Se puede insertar el elemento 5");

	pa2m_afirmar(abb_cantidad(abb) == 3, "El ABB tiene 3 elementos");

	abb_destruir_todo(abb, destructor_int);
}

void insertar_elementos_en_orden_diferente()
{
    abb_t *abb = abb_crear(comparador_int);
    int *elemento1 = malloc(sizeof(int));
    int *elemento2 = malloc(sizeof(int));
    int *elemento3 = malloc(sizeof(int));
    *elemento1 = 20, *elemento2 = 10, *elemento3 = 30;

    pa2m_afirmar(abb_insertar(abb, elemento1), "Se puede insertar el elemento 20");
    pa2m_afirmar(abb_insertar(abb, elemento2), "Se puede insertar el elemento 10");
    pa2m_afirmar(abb_insertar(abb, elemento3), "Se puede insertar el elemento 30");

    pa2m_afirmar(abb_cantidad(abb) == 3, "El ABB tiene 3 elementos");

    abb_destruir_todo(abb, destructor_int);
}

void insertar_elementos_duplicados()
{
    abb_t *abb = abb_crear(comparador_int);
    int *elemento1 = malloc(sizeof(int));
    int *elemento2 = malloc(sizeof(int));
    *elemento1 = 10, *elemento2 = 10;

    pa2m_afirmar(abb_insertar(abb, elemento1), "Se puede insertar el elemento 10");
    pa2m_afirmar(abb_insertar(abb, elemento2),  "Se puede insertar el elemento 10 duplicado");

    pa2m_afirmar(abb_cantidad(abb) == 2, "El ABB tiene 2 elemento");

    abb_destruir_todo(abb, destructor_int);
}

void insertar_multiples_elementos()
{
    abb_t *abb = abb_crear(comparador_int);
    int elementos[10];
    for (int i = 0; i < 10; i++) {
        elementos[i] = i;
        pa2m_afirmar(abb_insertar(abb, &elementos[i]), "Se puede insertar el elemento");
    }

    pa2m_afirmar(abb_cantidad(abb) == 10, "El ABB tiene 10 elementos");

    abb_destruir(abb);
}

// ----- QUITAR
void quitar_elementos_da_resultados_correctos()
{
	abb_t *abb = abb_crear(comparador_int);
	int *elemento1 = malloc(sizeof(int));
	int *elemento2 = malloc(sizeof(int));
	int *elemento3 = malloc(sizeof(int));
	*elemento1 = 10, *elemento2 = 20, *elemento3 = 5,

	abb_insertar(abb, elemento1);
	abb_insertar(abb, elemento2);
	abb_insertar(abb, elemento3);

	int *encontrado = NULL;
	pa2m_afirmar(abb_quitar(abb, elemento1, (void **)&encontrado),
		    "Se puede quitar el elemento 10");
	pa2m_afirmar(encontrado == elemento1,
		    "El elemento quitado es el correcto");
	free(encontrado);

	pa2m_afirmar(abb_cantidad(abb) == 2,
		"El ABB tiene 2 elementos después de quitar uno");

	abb_destruir_todo(abb, destructor_int);
}

void quitar_en_arbol_vacio_devuelve_false()
{
	abb_t *abb = abb_crear(comparador_int);
	int elemento = 10;
	void *encontrado = NULL;
	pa2m_afirmar(abb_quitar(abb, &elemento, &encontrado) == false,
		     "No se puede quitar de un árbol vacío");
	pa2m_afirmar(encontrado == NULL, "El elemento no se encontró");
	abb_destruir(abb);
}

void quitar_unico_elemento_resultado_correcto()
{
	abb_t *abb = abb_crear(comparador_int);
	int elemento = 10;
	abb_insertar(abb, &elemento);
	void *encontrado = NULL;
	pa2m_afirmar(abb_quitar(abb, &elemento, &encontrado) == true,
		     "Se puede quitar la raíz");
	pa2m_afirmar(encontrado == &elemento,
		     "El elemento quitado es el correcto");
	pa2m_afirmar(abb_cantidad(abb) == 0, "El árbol quedó vacío");
	abb_destruir(abb);
}

void quitar_hoja_resultado_correcto()
{
	abb_t *abb = abb_crear(comparador_int);
	int elem1 = 20, elem2 = 10;
	abb_insertar(abb, &elem1);
	abb_insertar(abb, &elem2); // Insertar hoja
	void *encontrado = NULL;
	pa2m_afirmar(abb_quitar(abb, &elem2, &encontrado) == true,
		     "Se puede quitar una hoja");
	pa2m_afirmar(encontrado == &elem2,
		     "Hoja quitada devuelta es la correcta");
	pa2m_afirmar(abb_cantidad(abb) == 1,
		     "La cantidad de elementos es la correcta");
	abb_destruir(abb);
}

void quitar_nodo_con_un_hijo()
{
	abb_t *abb = abb_crear(comparador_int);
	int elem1 = 20, elem2 = 10, elem3 = 5;
	abb_insertar(abb, &elem1);
	abb_insertar(abb, &elem2);
	abb_insertar(abb, &elem3); // Nodo con un solo hijo
	void *encontrado = NULL;
	pa2m_afirmar(abb_quitar(abb, &elem2, &encontrado) == true,
		    "Se puede quitar un nodo con un solo hijo");
	pa2m_afirmar(encontrado == &elem2,
		    "El elemento quitado es devuelto correctamente");
	pa2m_afirmar(abb_cantidad(abb) == 2,
		    "Cantidad de elementos es correcta");
	abb_destruir(abb);
}

void quitar_nodo_con_dos_hijos()
{
	abb_t *abb = abb_crear(comparador_int);
	int elem1 = 20, elem2 = 10, elem3 = 30, elem4 = 5, elem5 = 15;
	abb_insertar(abb, &elem1);
	abb_insertar(abb, &elem2);
	abb_insertar(abb, &elem3);
	abb_insertar(abb, &elem4);
	abb_insertar(abb, &elem5); // Nodo con dos hijos
	void *encontrado = NULL;
	pa2m_afirmar(abb_quitar(abb, &elem2, &encontrado) == true,
		     "Se puede quitar un nodo con dos hijos");
	pa2m_afirmar(encontrado == &elem2,
		     "Elemento quitado es devuelto correctamente");
	pa2m_afirmar(abb_cantidad(abb) == 4,
		     "Cantidad de elementos es la correcta");
	abb_destruir(abb);
}

// ----- OBTENER
void obtener_elementos_devuelve_resultados_correctos()
{
	abb_t *abb = abb_crear(comparador_int);
	int *elemento1 = malloc(sizeof(int));
	int *elemento2 = malloc(sizeof(int));
	int *elemento3 = malloc(sizeof(int));
	*elemento1 = 10, *elemento2 = 20, *elemento3 = 5;

	abb_insertar(abb, elemento1);
	abb_insertar(abb, elemento2);
	abb_insertar(abb, elemento3);

	pa2m_afirmar(abb_obtener(abb, elemento1) == elemento1,
		    "Se puede obtener el elemento 10");
	pa2m_afirmar(abb_obtener(abb, elemento2) == elemento2,
		    "Se puede obtener el elemento 20");
	pa2m_afirmar(abb_obtener(abb, elemento3) == elemento3,
		    "Se puede obtener el elemento 5");

	abb_destruir_todo(abb, destructor_int);
}

void obtener_elemento_inexistente_devuelve_null()
{
    abb_t *abb = abb_crear(comparador_int);
    int elemento1 = 10, elemento2 = 20;
    int elemento_inexistente = 15;

    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);

    pa2m_afirmar(abb_obtener(abb, &elemento_inexistente) == NULL,
                 "Obtener un elemento inexistente devuelve NULL");

    abb_destruir(abb);
}

void obtener_en_arbol_vacio_devuelve_null()
{
    abb_t *abb = abb_crear(comparador_int);
    int elemento = 10;

    pa2m_afirmar(abb_obtener(abb, &elemento) == NULL,
                 "Obtener en un árbol vacío devuelve NULL");

    abb_destruir(abb);
}

void obtener_elemento_en_raiz()
{
    abb_t *abb = abb_crear(comparador_int);
    int elemento = 10;

    abb_insertar(abb, &elemento);

    pa2m_afirmar(abb_obtener(abb, &elemento) == &elemento,
                 "Se puede obtener el elemento en la raíz");

    abb_destruir(abb);
}

void obtener_elemento_en_hoja()
{
    abb_t *abb = abb_crear(comparador_int);
    int elemento1 = 20, elemento2 = 10, elemento3 = 30;

    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    pa2m_afirmar(abb_obtener(abb, &elemento3) == &elemento3,
                 "Se puede obtener un elemento en una hoja");

    abb_destruir(abb);
}

void obtener_elemento_interno()
{
    abb_t *abb = abb_crear(comparador_int);
    int elemento1 = 20, elemento2 = 10, elemento3 = 30, elemento4 = 15;

    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);
    abb_insertar(abb, &elemento4);

    pa2m_afirmar(abb_obtener(abb, &elemento2) == &elemento2,
                 "Se puede obtener un elemento interno del árbol");

    abb_destruir(abb);
}

// ----- ITERADORES
void iterar_elementos_muestra_elementos()
{
	abb_t *abb = abb_crear(comparador_int);
	int *elemento1 = malloc(sizeof(int));
	int *elemento2 = malloc(sizeof(int));
	int *elemento3 = malloc(sizeof(int));
	*elemento1 = 10, *elemento2 = 20, *elemento3 = 5;

	abb_insertar(abb, elemento1);
	abb_insertar(abb, elemento2);
	abb_insertar(abb, elemento3);

	printf("Elementos en orden: ");
	abb_iterar_inorden(abb, imprimir_elemento, NULL);
	printf("\n");

	abb_destruir_todo(abb, destructor_int);
}


void iterar_inorden_aplica_funcion_correctamente()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 5;
    int elemento3 = 15;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    int suma = 0;
    abb_iterar_inorden(abb, sumar_elementos, &suma);

    pa2m_afirmar(suma == 30, "La suma de los elementos inorden es correcta");
    abb_destruir(abb);
}

void iterar_preorden_aplica_funcion_correctamente()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 5;
    int elemento3 = 15;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    int suma = 0;
    abb_iterar_preorden(abb, sumar_elementos, &suma);

    pa2m_afirmar(suma == 30, "La suma de los elementos preorden es correcta");
    abb_destruir(abb);
}

void iterar_postorden_aplica_funcion_correctamente()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 5;
    int elemento3 = 15;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    int suma = 0;
    abb_iterar_postorden(abb, sumar_elementos, &suma);

    pa2m_afirmar(suma == 30, "La suma de los elementos postorden es correcta");
    abb_destruir(abb);
}

// ----- VECTORIZADOR
void vectorizar_inorden_guarda_en_vector_correctamente()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 5;
    int elemento3 = 15;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    void* vector[3];
    size_t cantidad = abb_vectorizar_inorden(abb, vector, 3);

    pa2m_afirmar(cantidad == 3, "Vectorizar inorden guarda todos los elementos");
    pa2m_afirmar(*(int*)vector[0] == 5, "El primer elemento es correcto");
    pa2m_afirmar(*(int*)vector[1] == 10, "El segundo elemento es correcto");
    pa2m_afirmar(*(int*)vector[2] == 15, "El tercer elemento es correcto");

    abb_destruir(abb);
}

void vectorizar_preorden_guarda_en_vector_correctamente()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 5;
    int elemento3 = 15;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    void* vector[3];
    size_t cantidad = abb_vectorizar_preorden(abb, vector, 3);

    pa2m_afirmar(cantidad == 3, "Vectorizar preorden guarda todos los elementos");
    pa2m_afirmar(*(int*)vector[0] == 10, "El primer elemento es correcto");
    pa2m_afirmar(*(int*)vector[1] == 5, "El segundo elemento es correcto");
    pa2m_afirmar(*(int*)vector[2] == 15, "El tercer elemento es correcto");

    abb_destruir(abb);
}

void vectorizar_postorden_guarda_en_vector_correctamente()
{
    abb_t* abb = abb_crear(comparador_int);
    int elemento1 = 10;
    int elemento2 = 5;
    int elemento3 = 15;
    abb_insertar(abb, &elemento1);
    abb_insertar(abb, &elemento2);
    abb_insertar(abb, &elemento3);

    void* vector[3];
    size_t cantidad = abb_vectorizar_postorden(abb, vector, 3);

    pa2m_afirmar(cantidad == 3, "Vectorizar postorden guarda todos los elementos");
    pa2m_afirmar(*(int*)vector[0] == 5, "El primer elemento es correcto");
    pa2m_afirmar(*(int*)vector[1] == 15, "El segundo elemento es correcto");
    pa2m_afirmar(*(int*)vector[2] == 10, "El tercer elemento es correcto");

    abb_destruir(abb);
}

int main()
{
    pa2m_nuevo_grupo("Pruebas de ABB");
    prueba_crear_abb();

    // INSERTAR
    pa2m_nuevo_grupo("Pruebas funcion insertar");
    insertar_elementos_devuelve_cantidad_correcta();
    insertar_elementos_en_orden_diferente();
    insertar_elementos_duplicados();
    insertar_multiples_elementos();

    // QUITAR
    pa2m_nuevo_grupo("Pruebas funcion quitar");
    quitar_elementos_da_resultados_correctos();
    quitar_en_arbol_vacio_devuelve_false();
    quitar_hoja_resultado_correcto();
    quitar_nodo_con_un_hijo();
    quitar_nodo_con_dos_hijos();

    // OBTENER
    pa2m_nuevo_grupo("Pruebas funcion obtener");
    obtener_elementos_devuelve_resultados_correctos();
    obtener_elemento_inexistente_devuelve_null();
    obtener_en_arbol_vacio_devuelve_null();
    obtener_elemento_en_raiz();
    obtener_elemento_en_hoja();
    obtener_elemento_interno();

    // DESTRUIR
    pa2m_nuevo_grupo("Pruebas funcion destruir");
    destruir_abb_vacio_no_rompe();
    destruir_abb_con_elementos_no_rompe();
    destruir_abb_con_elementos_y_funcion_destructora();

    // ITERAR
    pa2m_nuevo_grupo("Pruebas de iteración");
    iterar_inorden_aplica_funcion_correctamente();
    iterar_preorden_aplica_funcion_correctamente();
    iterar_postorden_aplica_funcion_correctamente();

    // VECTORIZAR
    pa2m_nuevo_grupo("Pruebas de vectorización");
    vectorizar_inorden_guarda_en_vector_correctamente();
    vectorizar_preorden_guarda_en_vector_correctamente();
    vectorizar_postorden_guarda_en_vector_correctamente();

    return pa2m_mostrar_reporte();
}
