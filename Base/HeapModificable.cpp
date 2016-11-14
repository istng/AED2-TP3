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

HeapModificable::Iterador HeapModificable::encolar(const JugadorHeap& a)
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
	Nodo* destruir = tope;
	if ((*tope).hijoIzq == NULL && (*tope).hijoDer == NULL)
	{
		tope = NULL;
	}
	else
	{
		Nodo* ultimoNodo = ultimoNodo(tope);
		Nodo* padreUlt = (*ultimoNodo).padre;
		if ((padreUlt).hijoDer = ultimoNodo)
		{
			(padreUlt).hijoDer = NULL;
		}
		else
		{
			(padreUlt).hijoIzq = NULL;
		}
		corregirProfundidad(padreUlt, tope);
		(*ultimoNodo).padre = NULL;
		(*ultimoNodo).hijoIzq = (*tope).hijoIzq;
		(*ultimoNodo).hijoDer = (*tope).hijoDer;
		(*ultimoNodo).ramaMasCorta = (*tope).ramaMasCorta;
		(*ultimoNodo).ramaMasLarga = (*tope).ramaMasLarga;
		tope = ultimoNodo;
		siftDown(tope);
	}
	delete(tope);
}

// Iterador
HeapModificable::Iterador::Iterador();	// CrearIt()
{
	return tope;
}

bool HeapModificable::Iterador::haySiguiente() const;
{
	return tope != NULL;
}

JugadorHeap& HeapModificable::Iterador::Siguiente() const;
{
	return (*siguiente).elemento;
}

void HeapModificable::Iterador::eliminarSiguiente();
{
	Nodo* ultimoNodo = (*heap).tope;
	if ((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL)
	{
		(*heap).tope = NULL;
	}
	else
	{
		ultimoNodo = heap.ultimoNodo();	//en diseño: quedo de c, era del heap del it
		Nodo* padreUlt = (*ultimoNodo).padre;
		if ((*padreUlt).hijoDer == ultimoNodo)
		{
			(*padreUlt).hijoDer = NULL;
		}
		else
		{
			(*padreUlt).hijoIzq = NULL;
		}
		heap.corregirProfundidad(padreUlt);	//idem
		(*ultimoNodo).padre = (*siguiente).padre;
		(*ultimoNodo).hijoIzq = (*siguiente).hijoIzq;
		(*ultimoNodo).hijoDer = (*siguiente).hijoDer;
		(*ultimoNodo).ramaMasCorta = (*siguiente).ramaMasCorta;
		(*ultimoNodo).ramaMasLarga = (*siguiente).ramaMasLarga;
		if ((*(*siguiente).padre).hijoIzq == *siguiente)
		{
			(*(*siguiente).padre).hijoIzq = ultimoNodo;
		}
		else
		{
			(*(*siguiente).padre).hijoDer = ultimoNodo;
		}
		delete(siguiente);
		heap.siftDown(ultimoNodo);
		heap.siftUp(ultimoNodo);
	}
}

void HeapModificable::Iterador::agregarComoSiguiente(const JugadorHeap& a);
{
	return heap.encolar(a);
}


HeapModificable::const_Iterador::const_Iterador();	// CrearIt()
{
	return tope;
}


bool HeapModificable::const_Iterador::haySiguiente() const;
{
	return tope != NULL;
}

const JugadorHeap& HeapModificable::const_Iterador::Siguiente() const;
{
	return (*siguiente).elemento;
}

// private

Nodo* HeapModificable::ultimoNodo() const 
{
	Nodo* ultimoNodo = tope;
	while (!((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL))
	{
		if (*((*ultimoNodo).hijoIzq).ramaMasLarga == *((*ultimoNodo).hijoDer).ramaMasLarga)
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

Nodo* HeapModificable::futuroPadre() const 
{
	Nodo* ultimoNodo = tope;
	while (!((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL))
	{
		if (*((*ultimoNodo).hijoIzq).ramaMasLarga == *((*ultimoNodo).hijoDer).ramaMasLarga)
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
		(*p).ramaMasCorta = *((*p).hijoDer).ramaMasCorta + 1;
		(*p).ramaMasLarga = *((*p).hijoIzq).ramaMasLarga + 1;
	}
}

void HeapModificable::siftDown(Nodo* p)
{
	Nodo* swap = p;
	if (esMayor((*(*p).hijoIzq).elemento, (*p).elemento))
	{
		swap = (*p).hijoIzq;
	}
	if (esMayor((*(*p).hijoDer).elemento, (*p).elemento))
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

}

void HeapModificable::intercambio(Nodo* padre, Nodo* hijo)
{

}