#ifndef MAPA_H_
#define MAPA_H_

#include "TiposJuego.h"
#include "aed2/TiposBasicos.h"
#include "aed2/Vector.h"
#include "aed2/Conj.h"

using namespace aed2;

class Mapa
{
	public:
		//Generadores
	    Mapa();
	    ~Mapa();
		void agregarCoor(const Coordenada& c);

		//Observadores

		Conj<Coordenada>::const_Iterador coordenadas() const ;
		bool posExistente (const Coordenada& c) const ;
		bool hayCamino (const Coordenada& c , const Coordenada& cprima)const;
		bool hayCamino2 (const Coordenada& c , const Coordenada& cprima)const;
		bool ExisteCamino(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const;
		bool ExisteCaminoPorArriba(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const ;
		bool ExisteCaminoPorAbajo(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const;
		bool ExisteCaminoPorIzquierda(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const;
		bool ExisteCaminoPorDerecha(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs )const ;




	private:

		//Vector<Vector<Info>> columnas;
		//Conj<Coordenada> coords;

		/*struct Fila
		{
			vector<Info> fila;	
		};*/

		struct Info
		{
			bool _esta;
			Conj<Coordenada> _adyacentes;
			Info(): _esta(false), _adyacentes(){}				
			

		};



		Vector<Vector<Info> > columnas;
		Conj<Coordenada> coords;

	
};





#endif


