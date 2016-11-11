#ifndef TP3_HEAPMODIFICABLE_H
#define TP3_HEAPMODIFICABLE_H

#include "aed2/TiposBasicos.h"

using namespace aed2;

class HeapModificable
{
	private:
		struct Nodo;

	public:
	
	    class Iterador;
		class const_Iterador;
	
		struct JugadorHeap {
			Nat cantPokes;
			Nat id;
		};
		
		bool operator<(const JugadorHeap& b); // reemplaza función EsMayor?
	
	// Observadores
		bool esVacia(); // Vacia?()
		const JugadorHeap& proximo() const;
		
	// Generadores
		HeapModificable(); // Generador Vacia()
		Iterador Encolar(const JugadorHeap& a);
		
	// Otras operaciones
		void desencolar();
	
	// Iterador
		class Iterador
		{
			public:
				Iterador();	// CrearIt()
				bool haySiguiente() const;
				JugadorHeap& Siguiente() const;
				void eliminarSiguiente();
				void agregarComoSiguiente(const JugadorHeap& a);
				
			private:
				Nodo* siguiente;
				Nodo* heap;
		};
		
		class const_Iterador
		{
			public:
				Iterador();	// CrearIt()
				bool haySiguiente() const;
				const JugadorHeap& Siguiente() const;
				
			private:
				Nodo* siguiente;
				Nodo* heap;
		};
		
	private:
			
		struct Nodo
		{
			JugadorHeap elemento;
			Nat ramaMasCorta;
			Nat ramaMasLarga;
			Nodo* hijoIzq;
			Nodo* hijoDer;
			Nodo* padre;
		};
		
		Nodo* tope;
		
		Nodo* ultimoNodo() const;
		Nodo* futuroPadre() const;
		void corregirProfundidad(Nodo* p);
		void siftDown(Nodo* p);
		void siftUp(Nodo* p);
		void intercambio(Nodo* padre, Nodo* hijo);
};

#endif