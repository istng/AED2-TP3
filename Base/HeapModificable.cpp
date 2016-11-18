#include "HeapModificable.h"

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
			ftrPadre->hijoIzq = new Nodo(a, 0, 0, NULL, NULL, NULL);
			ftrPadre = ftrPadre->hijoIzq;
		}
		else {
			ftrPadre->hijoDer = new Nodo(a, 0, 0, NULL, NULL, NULL);
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
		if ((*padreUlt).hijoDer == ultNodo)
		{
			(*padreUlt).hijoDer = NULL;
		}
		else
		{
			(*padreUlt).hijoIzq = NULL;
		}
		corregirProfundidad(padreUlt);
		(*ultNodo).padre = NULL;
		(*ultNodo).hijoIzq = (*tope).hijoIzq;
		(*ultNodo).hijoDer = (*tope).hijoDer;
		(*ultNodo).ramaMasCorta = (*tope).ramaMasCorta;
		(*ultNodo).ramaMasLarga = (*tope).ramaMasLarga;
		tope = ultNodo;
		siftDown(tope);
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
	Nodo* ultimoNodo = (*heap).tope;
	if ((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL)
	{
		(*heap).tope = NULL;
	}
	else
	{
		ultimoNodo = heap->ultimoNodo();	//en diseño: quedo de c, era del heap del it
		Nodo* padreUlt = (*ultimoNodo).padre;
		if ((*padreUlt).hijoDer == ultimoNodo)
		{
			(*padreUlt).hijoDer = NULL;
		}
		else
		{
			(*padreUlt).hijoIzq = NULL;
		}
		heap->corregirProfundidad(padreUlt);	//idem
		(*ultimoNodo).padre = (*siguiente).padre;
		(*ultimoNodo).hijoIzq = (*siguiente).hijoIzq;
		(*ultimoNodo).hijoDer = (*siguiente).hijoDer;
		(*ultimoNodo).ramaMasCorta = (*siguiente).ramaMasCorta;
		(*ultimoNodo).ramaMasLarga = (*siguiente).ramaMasLarga;
		if ((*(*siguiente).padre).hijoIzq == siguiente)
		{
			(*(*siguiente).padre).hijoIzq = ultimoNodo;
		}
		else
		{
			(*(*siguiente).padre).hijoDer = ultimoNodo;
		}
		delete siguiente;
		heap->siftDown(ultimoNodo);
		heap->siftUp(ultimoNodo);
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
	while (!((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL))
	{
		if ( (ultimoNodo->hijoIzq)->ramaMasLarga == (ultimoNodo->hijoDer)->ramaMasLarga )
		{
			ultimoNodo = (*ultimoNodo).hijoDer;
		}
		else
		{
			ultimoNodo = (*ultimoNodo).hijoIzq;
		}
	}
	return ultimoNodo;
}

HeapModificable::Nodo* HeapModificable::futuroPadre() const 
{
	Nodo* ultimoNodo = tope;
	while (!((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL))
	{
		if ( (ultimoNodo->hijoIzq)->ramaMasLarga == (ultimoNodo->hijoDer)->ramaMasLarga )
		{
			ultimoNodo = (*ultimoNodo).hijoDer;
		}
		else
		{
			ultimoNodo = (*ultimoNodo).hijoIzq;
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
	p = (*p).padre;
	while((*p).padre != NULL)
	{
		(*p).ramaMasCorta = (*((*p).hijoDer)).ramaMasCorta + 1;
		(*p).ramaMasLarga = (*((*p).hijoIzq)).ramaMasLarga + 1;
	}
}

void HeapModificable::siftDown(Nodo* p)
{
	Nodo* swap = p;
	if (((*(*p).hijoIzq).elemento < (*p).elemento))
	{
		swap = (*p).hijoIzq;
	}
	if ((*(*p).hijoDer).elemento < (*p).elemento)
	{
		swap = (*p).hijoDer;
	}
	if (p != swap)
	{
		intercambio(p, swap);
		siftDown(swap);
	}
}

void HeapModificable::siftUp(Nodo* p)
{
	Nodo* swap = p;
	if(p->elemento < (p->padre)->elemento){
		swap = p->padre;
	}
	if(p != swap){
		intercambio(swap, p);
		siftUp(swap);
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
