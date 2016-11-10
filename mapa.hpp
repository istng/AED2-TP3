#ifndef MAPA_H_
#define MAPA_H_

#include "aed2/TiposJuego.h"

using namespace aed2

class Mapa
{
	public:
		//Generadores
		void crearMapa();
		void agregarCoor(const Coordenada& c);
		//Destructor
		void ~Mapa();

		//Observadores

		Conj<Coordenada>::Iterador coordenadas() const ;
		bool posExistente (const Coordenada& c) const ;
		bool hayCamino (const Coordenada& c , const Coordenada& cprima)const;
		bool hayCamino2 (const Coordenada& c , const Coordenada& cprima)const;
		bool ExisteCamino(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs );
		bool ExisteCaminoPorArriba(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs );
		bool ExisteCaminoPorAbajo(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs );
		bool ExisteCaminoPorIzquierda(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs );
		bool ExisteCaminoPorDerecha(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs );




	private:

		Vector<Vector<info>> columnas;
		Conj<Coordenada> coordenadas;

		/*struct Fila
		{
			vector<info> fila;	
		};*/

		struct Info
		{
			bool _esta;
			Conj _adyacentes;
			Info info(){
				_esta = false;
				_adyacentes = Conj(); 
			}

		};

	
};


void Mapa::crearMapa(){
	this->columnas = Vector();
	this->coordenadas = Conj();

}

Conj<Coordenada>::Iterador Mapa::coordenadas(){

	return this->coordenadas.Iterador();

}

bool Mapa::posExistente(const Coordenada& c){

	if c.latitud < longitud(this->columnas)
	{
		Vector<Info>& fila = this->columnas[c.latitud];
		if (c.longitud < longitud(fila))
		{
			return fila[c.longitud].esta;
		}
	}

	return false;
}

void Mapa::agregarCoor(const Coordenada& c){

	if longitud(this->columnas) < longitud(c)
	{
		int i = longitud(this->columnas);
		while (i <= longitud(c))
		{
			this->columnas.AgregarAtras(Vector());
			i++;
		}
	}

	Vector<Info>* fila = this->columnas[longitud(c)];
	if (longitud(&fila) < latitud(c))
	{
		j = longitud(&fila);
		while(j <= latitud(c))
		{
			&fila.AgregarAtras(info())
			j++;
		}
	}

	info* info = fila[latitud(c)];
	info._esta = true;

	Conj<Coordenada>::Iterador it = coordenadas(m);

	while (HaySiguiente(it))
	{
		Coordenada cprima = Siguiente(it);
		bool hayCamino = this->hayCamino2(c,cprima);
		bool enRango = c.distanciaEuclidea(Siguiente(it)) < 100;

		if ( hayCamino && enRango && c != cprima )
		{
			this->columnas[longitud(cprima)][latitud(cprima)]._adyacentes.AgregarRapido(c);
			info._adyacentes.AgregarRapido(cprima);
		}

		Avanzar(it);
	}

}

bool Mapa::hayCamino (const Coordenada& c , const Coordenada& cprima){

	if (c.distanciaEuclidea(c))
	{
		return this->columnas[longitud(c)][latitud(c)].Pertenece(cprima);
	}
	else
	{
		return this->hayCamino2(c,cprima);
	}

}

bool Mapa::hayCamino (const Coordenada& c , const Coordenada& cprima){

	Conj<Coordenada> coords = Conj(this->coordenadas);
	return this->ExisteCamino(c,cprima,coords);

}

bool Mapa::ExisteCamino(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ){

	if (c == cprima)
	{return true;}
	else
	{
		if (cs.EsVacio())
		{return false;}
		else
		{
			bool arriva = this->ExisteCaminoPorArriba(c,cprima,cs);
			bool abajo = this->ExisteCaminoPorAbajo(c,cprima,cs);
			bool izquierda = this->ExisteCaminoPorIzquierda(c,cprima,cs);
			bool derecha = this->ExisteCaminoPorDerecha(c,cprima,cs);

			return (arriva or abajo or izquierda or derecha);
		}
	}
}


bool Mapa::ExisteCaminoPorArriba(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ){

	if cs.Pertenece(c.coordenadaArriba())
	{
		return this->ExisteCamino(c.coordenadaArriba(),cprima,cs.Eliminar(c.coordenadaArriba()));

	}
	else
	{
		return false;
	}

}


bool Mapa::ExisteCaminoPorAbajo(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ){

	if cs.Pertenece(c.coordenadaAbajo())
	{
		return this->ExisteCamino(c.coordenadaAbajo(),cprima,cs.Eliminar(c.coordenadaAbajo()));

	}
	else
	{
		return false;
	}

}


bool Mapa::ExisteCaminoPorIzquierda(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ){

	if cs.Pertenece(c.coordenadaALaIzquierda())
	{
		return this->ExisteCamino(c.coordenadaALaIzquierda(),cprima,cs.Eliminar(c.coordenadaALaIzquierda()));

	}
	else
	{
		return false;
	}

}

bool Mapa::ExisteCaminoPorDerecha(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ){

	if cs.Pertenece(c.coordenadaALaDerecha())
	{
		return this->ExisteCamino(c.coordenadaALaDerecha(),cprima,cs.Eliminar(c.coordenadaALaDerecha()));

	}
	else
	{
		return false;
	}

}