#include "HeapModificable.h"

// JugadorHeap
bool HeapModificable::operator<(const JugadorHeap& b)
{ 
	return false;
}

// Observadores
bool HeapModificable::esVacia()
{
	return false;
}

const HeapModificable::JugadorHeap& HeapModificable::proximo() const 
{
	JugadorHeap* jh = new JugadorHeap();
	return *jh;
}

// Generadores
HeapModificable::HeapModificable() : tope(NULL) {}

HeapModificable::Iterador HeapModificable::Encolar(const JugadorHeap& a)
{
	if(tope == NULL){
		tope = new Nodo;
	}
	else {
		Nodo* ftrPadre = futuroPadre();
		if(ftrPadre->hijoIzq == NULL){
			ftrPadre->hijoIzq = new Nodo;
			ftrPadre = ftrPadre->hijoIzq;
		}
		else {
			ftrPadre->hijoDer = new Nodo;
			ftrPadre = ftrPadre->hijoDer;
		}
		corregirProfundidad(ftrPadre->padre);
		siftUp(ftrPadre);
	}
}

// Otras operaciones

// private

void HeapModificable::siftUp(Nodo* p)
{

}

void HeapModificable::intercambio(Nodo* padre, Nodo* hijo)
{

}
