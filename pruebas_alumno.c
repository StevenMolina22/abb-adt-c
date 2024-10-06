#include "pa2m.h"
#include "src/abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- FUNCIONES AUXILIARES
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

void prueba_crear_abb()
{
	pa2m_afirmar(abb_crear(comparador_int) != NULL,
		     "Se puede crear un ABB");
}

// --- INSERTAR
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

// --- QUITAR
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

	pa2m_afirmar(abb_cantidad(abb) == 2,
		"El ABB tiene 2 elementos después de quitar uno");

	abb_destruir_todo(abb, destructor_int);
}

// Test 1: Quitar de un árbol vacío
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

// Test 2: Quitar la raíz de un árbol con un solo elemento
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

// Test 3: Quitar una hoja (nodo sin hijos)
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

// Test 4: Quitar un nodo con un solo hijo
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

// Test 5: Quitar un nodo con dos hijos
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

// --- OBTENER
void prueba_obtener_elementos()
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

// --- ITERADORES
void prueba_iterar_elementos()
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

int main()
{
	pa2m_nuevo_grupo("Pruebas de ABB");
	prueba_crear_abb();

	pa2m_nuevo_grupo("Pruebas funcion insertar");
	insertar_elementos_devuelve_cantidad_correcta();

	// QUITAR
	pa2m_nuevo_grupo("Pruebas funcion quitar");
	quitar_elementos_da_resultados_correctos();
	quitar_en_arbol_vacio_devuelve_false();
	quitar_hoja_resultado_correcto();
	quitar_nodo_con_un_hijo();
	quitar_nodo_con_dos_hijos();

	// OBTENER
	pa2m_nuevo_grupo("Pruebas funcion obtener");
	prueba_obtener_elementos();
	prueba_iterar_elementos();

	return pa2m_mostrar_reporte();
}
