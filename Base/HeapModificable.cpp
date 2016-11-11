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
void HeapModificable::desencolar()
{

}

// Iterador
HeapModificable::Iterador::Iterador();	// CrearIt()
bool HeapModificable::Iterador::haySiguiente() const;
JugadorHeap& HeapModificable::Iterador::Siguiente() const;
void HeapModificable::Iterador::eliminarSiguiente();
void HeapModificable::Iterador::agregarComoSiguiente(const JugadorHeap& a);

HeapModificable::const_Iterador::const_Iterador();	// CrearIt()
bool HeapModificable::const_Iterador::haySiguiente() const;
const JugadorHeap& HeapModificable::const_Iterador::Siguiente() const;


// private

Nodo* HeapModificable::ultimoNodo() const 
{
	Nodo* n;
	return n;
}

Nodo* HeapModificable::futuroPadre() const 
{
	Nodo* n;
	return n;
}

void HeapModificable::corregirProfundidad(Nodo* p)
{

}

void HeapModificable::siftDown(Nodo* p)
{

}

void HeapModificable::siftUp(Nodo* p)
{

}

void HeapModificable::intercambio(Nodo* padre, Nodo* hijo)
{

}