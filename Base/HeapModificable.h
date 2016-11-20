#ifndef TP3_HEAPMODIFICABLE_H
#define TP3_HEAPMODIFICABLE_H

#include "aed2/TiposBasicos.h"

#include <iostream>
#include "mini_test.h"

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



// JugadorHeap
bool HeapModificable::JugadorHeap::operator<(const JugadorHeap& b)
{ 
	bool esMayor = false;
	if(cantPokes == b.cantPokes) esMayor = id < b.id;
	else esMayor = cantPokes < b.cantPokes;
	return esMayor;
}

// Observadores
bool HeapModificable::esVacia()
{
	return tope == NULL;
}

const HeapModificable::JugadorHeap& HeapModificable::proximo() const 
{
	return tope->elemento;
}

// Generadores
HeapModificable::HeapModificable() : tope(NULL) {}

HeapModificable::Iterador HeapModificable::encolar(const JugadorHeap& a)
{
    Nodo* siguienteIt;

	if(tope == NULL){
		tope = new Nodo(a, 0, 0, NULL, NULL, NULL);
		siguienteIt = tope;
	}
	else {
		Nodo* ftrPadre = futuroPadre();
		if(ftrPadre->hijoIzq == NULL){
			ftrPadre->hijoIzq = new Nodo(a, 0, 0, NULL, NULL, ftrPadre);
			ftrPadre = ftrPadre->hijoIzq;
		}
		else {
			ftrPadre->hijoDer = new Nodo(a, 0, 0, NULL, NULL, ftrPadre);
			ftrPadre = ftrPadre->hijoDer;
		}
		corregirProfundidad(ftrPadre->padre);
		siftUp(ftrPadre);
		siguienteIt = ftrPadre;
	}
	return Iterador(this, siguienteIt);
}

// Otras operaciones
void HeapModificable::desencolar()
{
	Nodo* destruir = tope;

	if ((*tope).hijoIzq == NULL && (*tope).hijoDer == NULL)
	{
		tope = NULL;
	}
	else
	{
		Nodo* ultNodo = ultimoNodo();
		Nodo* padreUlt = (*ultNodo).padre;
		
		if ((*padreUlt).hijoDer == ultNodo)	(*padreUlt).hijoDer = NULL;
		else (*padreUlt).hijoIzq = NULL;

		corregirProfundidad(padreUlt);

		(*ultNodo).padre = NULL;
		(*ultNodo).hijoIzq = (*tope).hijoIzq;
		(*ultNodo).hijoDer = (*tope).hijoDer;
		(*ultNodo).ramaMasCorta = (*tope).ramaMasCorta;
		(*ultNodo).ramaMasLarga = (*tope).ramaMasLarga;

		tope = ultNodo;

		if ((*tope).hijoIzq != NULL || (*tope).hijoDer != NULL)
		{
			siftDown(tope);
		}
	}
	
	delete destruir;
}

// Destructor
HeapModificable::~HeapModificable()
{
    while(tope != NULL){
        desencolar();
    }
}

// Iterador
HeapModificable::Iterador::Iterador(HeapModificable* h, HeapModificable::Nodo* siguiente)	// CrearIt()
	: heap(h), siguiente(siguiente)
{}

bool HeapModificable::Iterador::haySiguiente() const
{
	return heap->tope != NULL;
}

HeapModificable::JugadorHeap& HeapModificable::Iterador::Siguiente() const
{
	return (*siguiente).elemento;
}

void HeapModificable::Iterador::eliminarSiguiente()
{
	cout << endl << "elsig" << __LINE__ << endl;
	Nodo* ultimoNodo = (*heap).tope;
	cout << "elsig" << __LINE__ << endl;
	if ((*heap).tope == siguiente)
	{
		cout << "elsig" << __LINE__ << endl;
		(*heap).desencolar();
	}
	else
	{
		cout << "elsig" << __LINE__ << endl;
		ultimoNodo = heap->ultimoNodo();	//en diseño: quedo de c, era del heap del it
		cout << "elsig" << __LINE__ << endl;
		Nodo* padreUlt = (*ultimoNodo).padre;
		cout << "elsig" << __LINE__ << endl;
		if ((*padreUlt).hijoDer == ultimoNodo)
		{
			cout << "elsig" << __LINE__ << endl;
			(*padreUlt).hijoDer = NULL;
		}
		else
		{
			cout << "elsig" << __LINE__ << endl;
			(*padreUlt).hijoIzq = NULL;
		}
		cout << "elsig" << __LINE__ << endl;
		heap->corregirProfundidad(padreUlt);	//idem
		cout << "elsig" << __LINE__ << endl;
		(*ultimoNodo).padre = (*siguiente).padre;
		cout << "elsig" << __LINE__ << endl;
		(*ultimoNodo).hijoIzq = (*siguiente).hijoIzq;
		cout << "elsig" << __LINE__ << endl;
		(*ultimoNodo).hijoDer = (*siguiente).hijoDer;
		cout << "elsig" << __LINE__ << endl;
		(*ultimoNodo).ramaMasCorta = (*siguiente).ramaMasCorta;
		cout << "elsig" << __LINE__ << endl;
		(*ultimoNodo).ramaMasLarga = (*siguiente).ramaMasLarga;
		cout << "elsig" << __LINE__ << endl;
		if ((*siguiente).padre != NULL)
		{
			if ((*(*siguiente).padre).hijoIzq == siguiente)
			{
				cout << "elsig" << __LINE__ << endl;
				(*(*siguiente).padre).hijoIzq = ultimoNodo;
			}
			else
			{
				cout << "elsig" << __LINE__ << endl;
				(*(*siguiente).padre).hijoDer = ultimoNodo;
			}
		}
		cout << "elsig" << __LINE__ << endl;
		delete siguiente;
		cout << "elsig" << __LINE__ << endl;
		heap->siftDown(ultimoNodo);
		cout << "elsig" << __LINE__ << endl;
		heap->siftUp(ultimoNodo);
		cout << "elsig" << __LINE__ << endl;
	}
}

void HeapModificable::Iterador::agregarComoSiguiente(const JugadorHeap& a)
{
	heap->encolar(a);
}


HeapModificable::const_Iterador::const_Iterador(const HeapModificable* h, const HeapModificable::Nodo* siguiente) 	// CrearIt()
	: heap(h), siguiente(siguiente)
{}


bool HeapModificable::const_Iterador::haySiguiente() const
{
	return heap != NULL;
}

const HeapModificable::JugadorHeap& HeapModificable::const_Iterador::Siguiente() const
{
	return (*siguiente).elemento;
}

// private

HeapModificable::Nodo* HeapModificable::ultimoNodo() const 
{
	Nodo* ultimoNodo = tope;

	while ((*ultimoNodo).hijoIzq != NULL && (*ultimoNodo).hijoDer != NULL)
	{
		if ( (*(*ultimoNodo).hijoIzq).ramaMasLarga == (*(*ultimoNodo).hijoDer).ramaMasLarga )
		{
			ultimoNodo = (*ultimoNodo).hijoDer;
		}
		else
		{
			ultimoNodo = (*ultimoNodo).hijoIzq;
		}
	}
	if ((*ultimoNodo).hijoDer != NULL)
	{
		ultimoNodo = (*ultimoNodo).hijoDer;
	}
	else if((*ultimoNodo).hijoIzq != NULL)
	{
		ultimoNodo = (*ultimoNodo).hijoIzq;
	}

	return ultimoNodo;
}

HeapModificable::Nodo* HeapModificable::futuroPadre() const 
{
	Nodo* ultimoNodo = tope;
	while ((*ultimoNodo).hijoIzq != NULL && (*ultimoNodo).hijoDer != NULL)
	{
		if ( (ultimoNodo->hijoIzq)->ramaMasCorta == (ultimoNodo->hijoDer)->ramaMasCorta )
		{
			ultimoNodo = (*ultimoNodo).hijoIzq;
		}
		else
		{
			ultimoNodo = (*ultimoNodo).hijoDer;
		}
	}
	return ultimoNodo;
}

void HeapModificable::corregirProfundidad(Nodo* p)
{
	if ((*p).hijoIzq == NULL && (*p).hijoDer == NULL)
	{
		(*p).ramaMasCorta = 0;
		(*p).ramaMasLarga = 0;
	}
	else
	{
		if ((*p).hijoIzq != NULL && (*p).hijoDer != NULL)
		{
			(*p).ramaMasCorta = 1;
			(*p).ramaMasLarga = 1;
		}
		else
		{
			(*p).ramaMasCorta = 0;
			(*p).ramaMasLarga = 1;
		}
	}
	
	while((*p).padre != NULL)
	{
		p = (*p).padre;
		if ((*p).hijoDer != NULL)
		{
			(*p).ramaMasCorta = (*((*p).hijoDer)).ramaMasCorta + 1;
		}
		if ((*p).hijoIzq != NULL)
		{
			(*p).ramaMasLarga = (*((*p).hijoIzq)).ramaMasLarga + 1;
		}
		
	}
}

void HeapModificable::siftDown(Nodo* p)
{
	Nodo* swap = p;

	if ((*p).hijoIzq != NULL )
	{
		if (((*(*p).hijoIzq).elemento < (*p).elemento))
		{
		    swap = (*p).hijoIzq;
		}
	}

	if ((*p).hijoDer != NULL)
	{
		if ((*(*p).hijoDer).elemento < (*p).elemento)
		{
			swap = (*p).hijoDer;
		}
	}

	if (p != swap)
	{
		intercambio(p, swap);
		if(p == tope) tope = swap;
		siftDown(p);
	}
}

void HeapModificable::siftUp(Nodo* p)
{
	cout << "siftup" << __LINE__ << endl;
	if ((*p).padre == NULL){
		cout << "siftup" << __LINE__ << endl;
		tope = p;
	}
	else{
		cout << "siftup" << __LINE__ << endl;
		Nodo* swap = p;
		cout << "siftup" << __LINE__ << endl;
		if(p->elemento < (p->padre)->elemento){
			cout << "siftup" << __LINE__ << endl;
		    swap = p->padre;
	    }
	    cout << "siftup" << __LINE__ << endl;
	    bool juab = p != swap;
	    cout << juab << endl;
	    cout << "siftup" << __LINE__ << endl;
    	if(p != swap){
    		cout << "siftup" << __LINE__ << endl;
	    	intercambio(swap, p);
	    	cout << "siftup" << __LINE__ << endl;
		    siftUp(p);
	    }
	}
}

void HeapModificable::intercambio(Nodo* padre, Nodo* hijo)
{
	Nat rmc = hijo->ramaMasCorta;
	Nat rml = hijo->ramaMasLarga;
	hijo->padre = padre->padre;
	hijo->ramaMasCorta = padre->ramaMasCorta;
	hijo->ramaMasLarga = padre->ramaMasLarga;
	padre->padre = hijo;
	padre->ramaMasCorta = rmc;
	padre->ramaMasLarga = rml;
	
	if(padre->hijoIzq == hijo){
		Nodo* hDer = padre->hijoDer;
		padre->hijoIzq = hijo->hijoIzq;
		padre->hijoDer = hijo->hijoDer;
		hijo->hijoIzq = padre;
		hijo->hijoDer = hDer;
	} else {
		Nodo* hIzq = padre->hijoIzq;
		padre->hijoIzq = hijo->hijoIzq;
		padre->hijoDer = hijo->hijoDer;
		hijo->hijoIzq = hIzq;
		hijo->hijoDer = padre;
	}
}

// Nodo

HeapModificable::Nodo::Nodo(JugadorHeap jh, Nat rmc, Nat rml, Nodo* hI, Nodo* hD, Nodo* padre)
    : elemento(jh), ramaMasCorta(rmc), ramaMasLarga(rml), hijoIzq(hI), hijoDer(hD), padre(padre)
{}



#endif