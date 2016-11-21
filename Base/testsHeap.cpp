//#include <iostream>
#include "mini_test.h"

#include "HeapModificable.h"

#include <iostream>
using namespace std;

//auxiliares

bool jugHeapIgual(HeapModificable::JugadorHeap j1, HeapModificable::JugadorHeap j2)
{
	return j1.cantPokes == j2.cantPokes && j1.id == j2.id;
}



//Chequea que el diccionario vacio no tenga claves
void es_vacia() {
	HeapModificable h;
	ASSERT(h.esVacia());
}

void encolar() {

	HeapModificable h;

	HeapModificable::JugadorHeap j0 = HeapModificable::JugadorHeap(3, 0);

	h.encolar(j0);
	ASSERT(jugHeapIgual(h.proximo(), j0));

	HeapModificable::JugadorHeap j1 = HeapModificable::JugadorHeap(2, 1);
	HeapModificable::JugadorHeap j2 = HeapModificable::JugadorHeap(0, 2);
	HeapModificable::JugadorHeap j3 = HeapModificable::JugadorHeap(4, 3);
	h.encolar(j1);
	h.encolar(j2);
	h.encolar(j3);
	ASSERT(jugHeapIgual(h.proximo(), j2));
}

void eliminar_siguiente()
{
	HeapModificable h;

	HeapModificable::JugadorHeap j0 = HeapModificable::JugadorHeap(3, 0);

	h.encolar(j0);
	ASSERT(jugHeapIgual(h.proximo(), j0));

	HeapModificable::JugadorHeap j1 = HeapModificable::JugadorHeap(2, 1);
	HeapModificable::JugadorHeap j2 = HeapModificable::JugadorHeap(0, 2);
	HeapModificable::JugadorHeap j3 = HeapModificable::JugadorHeap(4, 3);
	HeapModificable::Iterador itj1 = h.encolar(j1);
	h.encolar(j2);
	h.encolar(j3);
	
	itj1.eliminarSiguiente();
	ASSERT(jugHeapIgual(h.proximo(), j2));


	h.desencolar();
	ASSERT(jugHeapIgual(h.proximo(), j0));

	h.desencolar();
	ASSERT(jugHeapIgual(h.proximo(), j3));

	h.desencolar();
	ASSERT(h.esVacia());

}

void siguiente()
{
	HeapModificable h;

	HeapModificable::JugadorHeap j0 = HeapModificable::JugadorHeap(3, 0);
	HeapModificable::JugadorHeap j1 = HeapModificable::JugadorHeap(2, 1);
	HeapModificable::JugadorHeap j2 = HeapModificable::JugadorHeap(0, 2);
	HeapModificable::JugadorHeap j3 = HeapModificable::JugadorHeap(4, 3);

	HeapModificable::Iterador itj0 = h.encolar(j0);
	HeapModificable::Iterador itj1 = h.encolar(j1);
	HeapModificable::Iterador itj2 = h.encolar(j2);
	HeapModificable::Iterador itj3 = h.encolar(j3);

	ASSERT(jugHeapIgual(itj0.Siguiente(), j0));
	ASSERT(jugHeapIgual(itj1.Siguiente(), j1));
	ASSERT(jugHeapIgual(itj2.Siguiente(), j2));
	ASSERT(jugHeapIgual(itj2.Siguiente(), h.proximo()));
	ASSERT(jugHeapIgual(itj3.Siguiente(), j3));

	itj1.eliminarSiguiente();
	ASSERT(jugHeapIgual(itj0.Siguiente(), j0));
	ASSERT(!jugHeapIgual(itj1.Siguiente(), j1));
	ASSERT(jugHeapIgual(itj2.Siguiente(), j2));
	ASSERT(jugHeapIgual(itj2.Siguiente(), h.proximo()));
	ASSERT(jugHeapIgual(itj3.Siguiente(), j3));

	itj2.eliminarSiguiente();
	ASSERT(jugHeapIgual(itj0.Siguiente(), j0));
	ASSERT(jugHeapIgual(itj0.Siguiente(), h.proximo()));
	ASSERT(!jugHeapIgual(itj2.Siguiente(), j2));
	ASSERT(!jugHeapIgual(itj2.Siguiente(), h.proximo()));
	ASSERT(jugHeapIgual(itj3.Siguiente(), j3));

}

void agregar_como_siguiente()
{/*
	HeapModificable h;
*/
	HeapModificable::JugadorHeap j0 = HeapModificable::JugadorHeap(3, 0);
	HeapModificable::JugadorHeap j1 = HeapModificable::JugadorHeap(2, 1);
	HeapModificable::JugadorHeap j2 = HeapModificable::JugadorHeap(0, 2);
	HeapModificable::JugadorHeap j3 = HeapModificable::JugadorHeap(4, 3);
/*
	HeapModificable::Iterador itj0 = h.encolar(j0);
	HeapModificable::Iterador itj3 = h.encolar(j3);

	itj3.agregarComoSiguiente(j1);
	ASSERT(jugHeapIgual(h.proximo(), j1));

	itj0.agregarComoSiguiente(j2);
	ASSERT(!jugHeapIgual(h.proximo(), j1));
	ASSERT(jugHeapIgual(h.proximo(), j2));
*/


	HeapModificable g;
	cout << endl << "agsig" << __LINE__ << endl;
	g.encolar(j1);
	cout << "agsig" << __LINE__ << endl;
	ASSERT(jugHeapIgual(g.proximo(), j1));
	cout << "agsig" << __LINE__ << endl;
	HeapModificable::Iterador itg3 = g.encolar(j3);
	cout << "agsig" << __LINE__ << endl;
	ASSERT(jugHeapIgual(g.proximo(), j1));
	cout << "agsig" << __LINE__ << endl;
	g.encolar(j2);
	cout << "agsig" << __LINE__ << endl;
	ASSERT(jugHeapIgual(g.proximo(), j2));
	cout << "agsig" << __LINE__ << endl;
	itg3.agregarComoSiguiente(j0);
	cout << "agsig" << __LINE__ << endl;
	ASSERT(jugHeapIgual(g.proximo(), j2));

}

int main() {
	RUN_TEST(es_vacia);
	RUN_TEST(encolar);
	RUN_TEST(eliminar_siguiente);
	RUN_TEST(siguiente);
	RUN_TEST(agregar_como_siguiente);

	return 0;

}
