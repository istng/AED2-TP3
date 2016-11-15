#ifndef TIPOSJUEGO_H_
#define TIPOSJUEGO_H_

#include "aed2/TiposBasicos.h"

using namespace aed2;

typedef unsigned int Jugador;
typedef std::string Pokemon;

struct Coordenada
{
	Nat latitud;
	Nat longitud;
	Coordenada(Nat latitud, Nat longitud) : latitud(latitud), longitud(longitud)
	{
	}
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

      

inline bool operator == (const Coordenada& c1, const Coordenada& c2){
 return c1.latitud == c2.latitud and c1.longitud == c2.longitud;
}


inline bool operator != (const Coordenada& c1, const Coordenada& c2){
 return not (c1 == c2);
}

inline std::ostream& operator<<(std::ostream& os, const Coordenada& c){
	os << '(' << c.latitud << ',' << c.longitud << ')';
}

#endif