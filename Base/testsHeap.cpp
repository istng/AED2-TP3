#include <iostream>
#include "mini_test.h"

#include "HeapModificable.h"

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
	//cout << endl << "assert0" << endl;
	ASSERT(jugHeapIgual(h.proximo(), j0));
	//cout << endl << "creacion jh" << endl;
	HeapModificable::JugadorHeap j1 = HeapModificable::JugadorHeap(2, 1);
	HeapModificable::JugadorHeap j2 = HeapModificable::JugadorHeap(0, 2);
	HeapModificable::JugadorHeap j3 = HeapModificable::JugadorHeap(4, 3);
		//cout << endl << "encolado 1" << endl;
	h.encolar(j1);
				//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
		//cout << j2.cantPokes << " " << j2.id << endl;
		//cout << endl << "encolado 2" << endl;
	h.encolar(j2);
			//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
		//cout << j2.cantPokes << " " << j2.id << endl;
		//cout << endl << "encolado 3" << endl;
	h.encolar(j3);
		//cout << endl << "assert1" << endl;
		//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
		//cout << j2.cantPokes << " " << j2.id << endl;
	ASSERT(jugHeapIgual(h.proximo(), j2));
	//cout << "imposible" << endl;
}

void eliminar_siguiente()
{
	HeapModificable h;

	HeapModificable::JugadorHeap j0 = HeapModificable::JugadorHeap(3, 0);

	h.encolar(j0);
	//cout << endl << "assert0" << endl;
	ASSERT(jugHeapIgual(h.proximo(), j0));
	//cout << endl << "creacion jh" << endl;
	HeapModificable::JugadorHeap j1 = HeapModificable::JugadorHeap(2, 1);
	HeapModificable::JugadorHeap j2 = HeapModificable::JugadorHeap(0, 2);
	HeapModificable::JugadorHeap j3 = HeapModificable::JugadorHeap(4, 3);
		//cout << endl << "encolado 1" << endl;
	HeapModificable::Iterador itj1 = h.encolar(j1);
				//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
		//cout << j2.cantPokes << " " << j2.id << endl;
		//cout << endl << "encolado 2" << endl;
	h.encolar(j2);
			//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
		//cout << j2.cantPokes << " " << j2.id << endl;
		//cout << endl << "encolado 3" << endl;
	h.encolar(j3);
	
	//cout << endl << "elsig1" << endl;
	itj1.eliminarSiguiente();
	//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
	//cout << j2.cantPokes << " " << j2.id << endl;
	
	//cout << "elsig2" << endl;
	ASSERT(jugHeapIgual(h.proximo(), j2));

	//cout << "elsig3" << endl;
	h.desencolar();
	//cout << h.proximo().cantPokes << " " << h.proximo().id << endl;
	//cout << j0.cantPokes << " " << j0.id << endl;
	
	//cout << "elsig4" << endl;
	ASSERT(jugHeapIgual(h.proximo(), j0));
	//cout << "elsig5" << endl;
	h.desencolar();
	//cout << "elsig6" << endl;
	ASSERT(jugHeapIgual(h.proximo(), j3));
	//cout << "elsig7" << endl;
	h.desencolar();
	//cout << "elsig8" << endl;
	
	ASSERT(h.esVacia());

}

int main() {
	RUN_TEST(es_vacia);
	RUN_TEST(encolar);
	RUN_TEST(eliminar_siguiente);

	return 0;

}
