#ifndef DICC_STRING_H_
#define DICC_STRING_H_

#include "aed2/Conj.h"
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

using namespace aed2;



//template<typename T>
//class Conj;

template<typename T>
class DiccString {
        public:
                /**
                CONSTRUCTOR
                * Construye un diccionario vacio.
                **/
                DiccString();

                /**
                CONSTRUCTOR POR COPIA
                * Construye un diccionario por copia.
                **/
                DiccString(const DiccString<T>&);


                /**
                DESTRUCTOR
                **/
                ~DiccString();

                /**
                DEFINIR
                * Recibe una clave con su significado de tipo T y la define.
                * Si ya estaba definida, la reescribe.
                **/
                void Definir(const string& clave, const T& significado);

                /**
                DEFINIDO?
                * Devuelve un bool, que es true si la clave pasada está definida en
                * el diccionario.
                **/
                bool Definido(const string& clave) const;

                /**
                OBTENER
                * Dada una clave, devuelve su significado.
                * PRE: La clave está definida.
                --PRODUCE ALIASING--
				-- Versión modificable y no modificable
                **/
				const T& Obtener(const string& clave) const;
                T& Obtener(const string& clave);

                /**
                OBTENER
                * Dada una clave, la borra del diccionario junto a su significado.
                * PRE: La clave está definida.
                --PRODUCE ALIASING--
                **/
                void Borrar(const string& clave);


                /**
                CLAVES
                * Devuelve las claves del diccionario.
                --NO PRODUCE ALIASING--
                **/
                aed2::Conj<string>& Claves();


        private:

                struct Nodo{
                    Nodo** siguientes;
                    T* definicion;
                    Nodo(){
						siguientes = new Nodo*[256];
                        for (unsigned int  i = 0; i < 256; i++){
                            siguientes[i] = NULL;
                        }
                        definicion = NULL;
                    }
                    ~Nodo(){
                        delete definicion;
						delete [] siguientes;
                    }
                };

                //TODO: funciones auxiliares

                unsigned int  cantidadNoNull(const Nodo* siguientes);
                unsigned int buscadorRuptura(Nodo* raiz, const string& clave, Nodo*& ruptura);
                void buscadorNodoDef(Nodo* raiz, const string& clave, Nodo*& nodoDef);
                void borrarDesde(Nodo* actual, const string& clave, unsigned int i);

                Nodo* raiz;
                Conj<string> claves;
};



template <typename T>
DiccString<T>::DiccString()
    : raiz(NULL), claves(){
}

//CAMBIAR MINIMO Y REMOVER MINIMO POR CREARIT, ETC
template <typename T>
DiccString<T>::DiccString(const DiccString& d) {
	
	raiz = NULL;
    Conj<string> c(d.claves);
    Conj<string>::Iterador itClaves = c.CrearIt();
    while(itClaves.HaySiguiente())
    {

    	string clave = itClaves.Siguiente();

    	T significado = d.Obtener(itClaves.Siguiente());
    	Definir(clave, significado);
        itClaves.EliminarSiguiente();
    }
}


template <typename T>
DiccString<T>::~DiccString(){
    Conj<string> c(claves);
    Conj<string>::Iterador itClaves = c.CrearIt();
    while(itClaves.HaySiguiente())
    {
        Borrar(itClaves.Siguiente());
        itClaves.EliminarSiguiente();
    }
    Nodo* destruir = raiz;
    raiz = NULL;
    delete(destruir);
}

template <typename T>
void DiccString<T>::Definir(const string& clave, const T& significado){
    
    //lo agrego al conjunto de claves si no estava
    if (!Definido(clave))
    {
        claves.AgregarRapido(clave);
    }

    //este actual lo uso para recorrer
    Nodo* actual = raiz;
	unsigned int  i = 0;

	//en el caso en que no haya nada
    if (raiz == NULL)
   	{
        Nodo* inicio = new Nodo;
        raiz = inicio;
        actual = raiz;
   	}
    else
    {
        while(i < clave.length())
        {
            //avanzo en el trie y en la clave, hasta llegar a la ultima pos donde hay algo
            if (actual->siguientes[int(clave[i])] ==  NULL)
            {
                break;
            }
            actual = actual->siguientes[int(clave[i])];
            i++;
        }
    }
   		
    //acá sólo se mete si la longitud de la clave es mayor a cero, y si (luego de
    //haber entrado al else anterior) llega a alguna hoja mas halla de la raiz.
    if (i == clave.length())
    {
    	T* destruir = actual->definicion;
		actual->definicion = NULL;
		delete(destruir);
    }
    else
    {
    	while(i < clave.length())
   		{
    	   Nodo* nuevo = new Nodo;
    	   actual->siguientes[int(clave[i])] = nuevo;
    	   //avanzo en el trie y en la clave
    	   actual = actual->siguientes[int(clave[i])];
		   i++;
		}
	}

	//agrego la def al lugar indicado
	T* nuevaDef = new T(significado);
	actual->definicion = nuevaDef;
}


template <typename T>
bool DiccString<T>::Definido(const string& clave) const{

	return claves.Pertenece(clave);

}

template <typename T>
T& DiccString<T>::Obtener(const string& clave) {

    Nodo* actual = raiz;
    //si no es la clave "", lo busco
    if (clave != "")
    {
        unsigned int  i = 0;
        while(i < clave.length())
        {
            //avanzo en el trie y en la clave
            actual = actual->siguientes[int(clave[i])];
            i++;
        }
    }

	return *actual->definicion; 

}


template <typename T>
const T& DiccString<T>::Obtener(const string& clave) const {

    Nodo* actual = raiz;
    //si no es la clave "", lo busco
    if (clave != "")
    {
        unsigned int  i = 0;
        while(i < clave.length())
        {
            //avanzo en el trie y en la clave
            actual = actual->siguientes[int(clave[i])];
            i++;
        }
    }

	return *actual->definicion; 

}


template <typename T>
Conj<string>& DiccString<T>::Claves() {
    return claves;
}


template <typename T>
void DiccString<T>::Borrar(const string& clave) {
    Nodo* ruptura = raiz;
    unsigned int letraRuptura = buscadorRuptura(raiz, clave, ruptura);
    Nodo* nodoDef = raiz;
    buscadorNodoDef(raiz, clave, nodoDef);

    if (ruptura == raiz)
    {
        if (cantidadNoNull(nodoDef) == 0)
        {
            if (cantidadNoNull(raiz) == 0)
            {
                //borro todo
                borrarDesde(raiz, clave, 0);
                Nodo* destruir = raiz;
                raiz = NULL;
                delete(destruir);
            }
            else
            {
                //borro todo, menos la raíz
                borrarDesde(raiz, clave, 0);
            }
        }
        else
        {
            //borro sólo la def
            T* destruir = nodoDef->definicion;
            nodoDef->definicion = NULL;
            delete(destruir);
        }
    }
    else
    {
        if (cantidadNoNull(nodoDef) == 0)
        {
            //borro hasta la ruptura, sin incluir
            borrarDesde(ruptura, clave, letraRuptura);
        }
        else
        {
            //borro sólo la def
            T* destruir = nodoDef->definicion;
            nodoDef->definicion = NULL;
            delete(destruir);
        }
    }
    claves.Eliminar(clave);
}

template <typename T>
unsigned int DiccString<T>::buscadorRuptura(Nodo* raiz, const string& clave, Nodo*& ruptura) {
    Nodo* actual = raiz;
    unsigned int i = 0;
    unsigned int letraRuptura = 0;
    while(i < clave.length())
    {
        if (cantidadNoNull(actual) > 1 || actual->definicion != NULL)
        {
            ruptura = actual;
            letraRuptura = i;
        }
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    return letraRuptura;
}

template <typename T>
void DiccString<T>::buscadorNodoDef(Nodo* raiz, const string& clave, Nodo*& nodoDef) {
    Nodo* actual = raiz;
    unsigned int i = 0;
    while(i < clave.length())
    {
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    nodoDef = actual;
}


template <typename T>
unsigned int  DiccString<T>::cantidadNoNull(const Nodo* siguientes) {

    unsigned int  contador = 0;
    unsigned int  i = 0;
    while(i < 256)
    {
        if (siguientes->siguientes[i] != NULL)
        {
            contador++;
        }
        i++;
    }
    return contador;
}

template <typename T>
void DiccString<T>::borrarDesde(Nodo* actual, const string& clave, unsigned int i) {
    unsigned int letraRuptura = i;
    while(i < clave.length())
    {
        if (i == clave.length() - 1)
        {
            Nodo* destruir = actual->siguientes[int(clave[letraRuptura])];
            actual->siguientes[int(clave[letraRuptura])] = NULL;
            delete(destruir);
        }
        else if (i < clave.length() - 1)
        {
            Nodo* destruir = actual->siguientes[int(clave[letraRuptura])];
            actual->siguientes[int(clave[letraRuptura])] = destruir->siguientes[int(clave[i+1])];
            delete(destruir);
        }
        i++;
    }
}

#endif
