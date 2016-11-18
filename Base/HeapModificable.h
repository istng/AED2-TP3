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
	////cout << endl << "enc1" << endl;
    Nodo* siguienteIt;
    ////cout << "enc2" << endl;
	if(tope == NULL){
		////cout << "enc3" << endl;
		tope = new Nodo(a, 0, 0, NULL, NULL, NULL);
		////cout << "enc4" << endl;
		siguienteIt = tope;
	}
	else {
		////cout << "enc5" << endl;
		Nodo* ftrPadre = futuroPadre();
		////cout << "enc6" << endl;
		if(ftrPadre->hijoIzq == NULL){
			////cout << "enc7" << endl;
			ftrPadre->hijoIzq = new Nodo(a, 0, 0, NULL, NULL, ftrPadre);
			////cout << "enc8" << endl;
			ftrPadre = ftrPadre->hijoIzq;

		}
		else {
			////cout << "enc9" << endl;
			ftrPadre->hijoDer = new Nodo(a, 0, 0, NULL, NULL, ftrPadre);
			////cout << "enc10" << endl;
			ftrPadre = ftrPadre->hijoDer;
		}
		////cout << "enc11" << endl;
		corregirProfundidad(ftrPadre->padre);
		////cout << "enc12" << endl;
		siftUp(ftrPadre);
		////cout << "enc13" << endl;
		siguienteIt = ftrPadre;
	}
	return Iterador(this, siguienteIt);
}

// Otras operaciones
void HeapModificable::desencolar()
{
	cout << "desenc1" << endl;
	Nodo* destruir = tope;
	cout << "desenc2" << endl;
	if ((*tope).hijoIzq == NULL && (*tope).hijoDer == NULL)
	{
		cout << "desenc3" << endl;
		tope = NULL;
	}
	else
	{
		cout << "desenc4" << endl;
		Nodo* ultNodo = ultimoNodo();
		bool JUAB = false;
		if (ultNodo == NULL)
		{
			JUAB = true;
		}
		cout << JUAB << endl;
		cout << "desenc5" << endl;
		Nodo* padreUlt = (*ultNodo).padre;
		cout << "desenc6" << endl;
		if ((*padreUlt).hijoDer == ultNodo)
		{
			cout << "desenc7" << endl;
			(*padreUlt).hijoDer = NULL;
		}
		else
		{
			cout << "desenc8" << endl;
			(*padreUlt).hijoIzq = NULL;
		}
		cout << "desenc9" << endl;
		corregirProfundidad(padreUlt);
		cout << "desenc10" << endl;
		(*ultNodo).padre = NULL;
		cout << "desenc11" << endl;
		(*ultNodo).hijoIzq = (*tope).hijoIzq;
		cout << "desenc12" << endl;
		(*ultNodo).hijoDer = (*tope).hijoDer;
		cout << "desenc13" << endl;
		(*ultNodo).ramaMasCorta = (*tope).ramaMasCorta;
		cout << "desenc14" << endl;
		(*ultNodo).ramaMasLarga = (*tope).ramaMasLarga;
		cout << "desenc15" << endl;
		tope = ultNodo;
		cout << "desenc16" << endl;
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
	////cout << "LUCAS NO TIENE RAZON" << endl;
    while(tope != NULL){
    	////cout << "LUCAS NO TIENE RAZON" << endl;
        desencolar();
        ////cout << "LUCAS NO TIENE RAZON" << endl;
    }
    ////cout << "SE EQUIVOCO" << endl;
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
	//cout << "elSIG1" << endl;
	Nodo* ultimoNodo = (*heap).tope;
	//cout << "elSIG2" << endl;
	if ((*ultimoNodo).hijoIzq == NULL && (*ultimoNodo).hijoDer == NULL)
	{
		//cout << "elSIG3" << endl;
		(*heap).tope = NULL;
	}
	////cout << "elSIG4" << endl;
	else
	{
		//cout << "elSIG5" << endl;
		ultimoNodo = heap->ultimoNodo();	//en diseño: quedo de c, era del heap del it
		//cout << "elSIG6" << endl;
		Nodo* padreUlt = (*ultimoNodo).padre;
		//cout << "elSIG7" << endl;
		if ((*padreUlt).hijoDer == ultimoNodo)
		{
			//cout << "elSIG7" << endl;
			(*padreUlt).hijoDer = NULL;
		}
		////cout << "elSIG8" << endl;
		else
		{
			//cout << "elSIG9" << endl;
			(*padreUlt).hijoIzq = NULL;
		}
		//cout << "elSIG10" << endl;
		heap->corregirProfundidad(padreUlt);	//idem
		//cout << "elSIG11" << endl;
		(*ultimoNodo).padre = (*siguiente).padre;
		//cout << "elSIG12" << endl;
		(*ultimoNodo).hijoIzq = (*siguiente).hijoIzq;
		//cout << "elSIG13" << endl;
		(*ultimoNodo).hijoDer = (*siguiente).hijoDer;
		//cout << "elSIG14" << endl;
		(*ultimoNodo).ramaMasCorta = (*siguiente).ramaMasCorta;
		//cout << "elSIG15" << endl;
		(*ultimoNodo).ramaMasLarga = (*siguiente).ramaMasLarga;
		//cout << "elSIG16" << endl;
		if ((*(*siguiente).padre).hijoIzq == siguiente)
		{
			//cout << "elSIG17" << endl;
			(*(*siguiente).padre).hijoIzq = ultimoNodo;
		}
		////cout << "elSIG18" << endl;
		else
		{
			//cout << "elSIG19" << endl;
			(*(*siguiente).padre).hijoDer = ultimoNodo;
		}
		//cout << "elSIG20" << endl;
		delete siguiente;
		//cout << "elSIG21" << endl;
		heap->siftDown(ultimoNodo);
		//cout << "elSIG22" << endl;
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
	////cout << "ult1" << endl;
	Nodo* ultimoNodo = tope;
	////cout << "ult2" << endl;
	while ((*ultimoNodo).hijoIzq != NULL && (*ultimoNodo).hijoDer != NULL)
	{
		////cout << "ult3" << endl;
		if ( (*(*ultimoNodo).hijoIzq).ramaMasLarga == (*(*ultimoNodo).hijoDer).ramaMasLarga )
		{
			////cout << "ult4" << endl;
			ultimoNodo = (*ultimoNodo).hijoDer;
		}
		else
		{
			////cout << "ult5" << endl;
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
	////cout << "ult6" << endl;

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
	////cout << "corr1" << endl;
	if ((*p).hijoIzq == NULL && (*p).hijoDer == NULL)
	{
		////cout << "corr2" << endl;
		(*p).ramaMasCorta = 0;
		(*p).ramaMasLarga = 0;
	}
	else
	{
		////cout << "corr3" << endl;
		if ((*p).hijoIzq != NULL && (*p).hijoDer != NULL)
		{
			////cout << "corr4" << endl;
			(*p).ramaMasCorta = 1;
			(*p).ramaMasLarga = 1;
		}
		else
		{
			////cout << "corr5" << endl;
			(*p).ramaMasCorta = 0;
			(*p).ramaMasLarga = 1;
		}
	}
	////cout << "corr6" << endl;
	////cout << "SON UNSIGNGSSEEDDEED" << endl;
	while((*p).padre != NULL)
	{
		////cout << "sera aca?" << endl;
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
	////cout << "corr324132143214" << endl;
}

void HeapModificable::siftDown(Nodo* p)
{
	//cout << "siftd1" << endl;
	Nodo* swap = p;
	//cout << "siftd2" << endl;
	if ((*p).hijoIzq != NULL )
	{
		if (((*(*p).hijoIzq).elemento < (*p).elemento))
		{
		//cout << "siftd3" << endl;
		swap = (*p).hijoIzq;
		}
	}

	if ((*p).hijoDer != NULL)
	{
		if ((*(*p).hijoDer).elemento < (*p).elemento)
		{
			//cout << "siftd5" << endl;
			swap = (*p).hijoDer;
		}
	}
	//cout << "siftd4" << endl;

	//cout << "siftd6" << endl;
	if (p != swap)
	{
		//cout << "siftd7" << endl;
		intercambio(p, swap);
		//cout << "siftd8" << endl;
		siftDown(swap);
	}
}

void HeapModificable::siftUp(Nodo* p)
{
	//cout << "siftu1" << endl;
	Nodo* swap = p;
	//cout << "siftu2" << endl;
	if(p->elemento < (p->padre)->elemento){
		//cout << "siftu3" << endl;
		swap = p->padre;
	}
	//cout << "siftu4" << endl;
	if(p != swap){
		//cout << "siftu5" << endl;
		intercambio(swap, p);
		//cout << "siftu6" << endl;
		siftUp(swap);
	}
	//cout << "siftu7" << endl;
	if ((*p).padre == NULL)
	{
		//cout << "siftu8" << endl;
		tope = p;
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
