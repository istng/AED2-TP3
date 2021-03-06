#ifndef TP3_HEAPMODIFICABLE_H
#define TP3_HEAPMODIFICABLE_H

#include "aed2/TiposBasicos.h"


#include <iostream>
using namespace std;

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
            JugadorHeap(Nat c, Nat i): cantPokes(c),id(i){}
            
            bool operator<(const JugadorHeap& b); // reemplaza funci?n EsMayor?
		};
		
	
	// Observadores
		bool esVacia(); // Vacia?()
		const JugadorHeap& proximo() const;
		
	// Generadores
		HeapModificable(); // Generador Vacia()
		Iterador encolar(const JugadorHeap& a);
		
	// Otras operaciones
		void desencolar();	//por algun motivo, esto esta como obs en el tad... ??
		
	// Destructor
	    ~HeapModificable();
	
	// Iterador
		class Iterador
		{
			public:
				Iterador(HeapModificable* h, HeapModificable::Nodo* siguiente);	// CrearIt()
				bool haySiguiente() const;
				JugadorHeap& Siguiente() const;
				void eliminarSiguiente();
				void agregarComoSiguiente(const JugadorHeap& a);

				// Operador de asignacion;
				HeapModificable::Iterador& operator = (const HeapModificable::Iterador& otra);

				
			private:
			    HeapModificable* heap;
				Nodo* siguiente;
		};
		
		class const_Iterador
		{
			public:
				const_Iterador(const HeapModificable* h, const HeapModificable::Nodo* siguiente);	// CrearIt()
				bool haySiguiente() const;
				const JugadorHeap& Siguiente() const;

				const HeapModificable::const_Iterador& operator = (const HeapModificable::const_Iterador& otra);
				
			private:
			    const HeapModificable* heap;
				const Nodo* siguiente;
		};
		
	private:
			
		struct Nodo
		{
		    Nodo(JugadorHeap jh, Nat rmc, Nat rml, Nodo* hI, Nodo* hD, Nodo* padre);
		
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