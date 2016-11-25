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

inline Nat latitud(const Coordenada& c){
	return c.latitud;
}

inline Nat longitud(const Coordenada& c){
	return c.longitud;
}


inline Nat DistEuclidea(const Coordenada& c1,const Coordenada& c2){
	Nat lon = 0;
	Nat lat = 0;
	if (c1.longitud > c2.longitud)
	{
		lon = (c1.longitud-c2.longitud)*(c1.longitud-c2.longitud);
	}
	else
	{
		lon = (c2.longitud-c1.longitud)*(c2.longitud-c1.longitud);
	}

	if (c1.latitud > c2.latitud)
	{
		lat = (c1.latitud-c2.latitud)*(c1.latitud-c2.latitud);
	}

	else
	{
		lat = (c2.latitud-c1.latitud)*(c2.latitud-c1.latitud);
	}

	return (lat + lon);


}


inline Coordenada CoordenadaArriba(const Coordenada& c){
	return Coordenada(latitud(c)+1,longitud(c));
}

inline Coordenada CoordenadaAbajo(const Coordenada& c){
	return Coordenada(latitud(c)-1,longitud(c));
} 

inline Coordenada CoordenadaALaDerecha(const Coordenada& c){
	return Coordenada(latitud(c),longitud(c)+1);
}
inline Coordenada CoordenadaALaIzquierda(const Coordenada& c){
	return Coordenada(latitud(c),longitud(c)-1);
}





#endif


