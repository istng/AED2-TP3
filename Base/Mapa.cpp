#include "Mapa.h"



Mapa::Mapa(): columnas(),coords(){
}

Mapa::~Mapa(){ }

Conj<Coordenada>::const_Iterador Mapa::coordenadas() const{

	return (this->coords).CrearIt();

}

bool Mapa::posExistente(const Coordenada& c) const{

	if (c.longitud < this->columnas.Longitud() )
	{
		//std::cout << std::endl << c.longitud <<" " << this->columnas.Longitud() <<std::endl;
		Vector<Info> fila = this->columnas[c.longitud];
		if (c.latitud < fila.Longitud())

		{
			return fila[c.latitud]._esta;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void Mapa::agregarCoor(const Coordenada& c){

	this->coords.Agregar(c);

	if (this->columnas.Longitud() < longitud(c) + 1) 
	{
		Nat i = this->columnas.Longitud();
		while (i <= longitud(c) + 1)
		{
			Vector<Info> v;
			this->columnas.AgregarAtras(v);
			i++;
		}
	}

	Vector<Info>* fila = &(this->columnas[longitud(c)]);
	if (fila->Longitud() < latitud(c) + 1)
	{
		Nat j = fila->Longitud();
		while(j <= latitud(c) + 1)
		{
			fila->AgregarAtras(Info());
			j++;
		}
	}

	Info* Info = & (this->columnas[longitud(c)][latitud(c)]);
	Info->_esta = true;

	Conj<Coordenada>::const_Iterador it = this->coordenadas();

	while (it.HaySiguiente())
	{
		Coordenada cprima = it.Siguiente();
		bool hayCamino = this->hayCamino2(c,cprima);
		bool enRango = DistEuclidea(c,it.Siguiente()) < 100;

		if ( hayCamino && enRango && c != cprima )
		{
			this->columnas[longitud(cprima)][latitud(cprima)]._adyacentes.AgregarRapido(c);
			Info->_adyacentes.AgregarRapido(cprima);
		}

		it.Avanzar();
	}

	//std::cout << std::endl << this->coords << std:: endl;

}

bool Mapa::hayCamino (const Coordenada& c , const Coordenada& cprima)const {

	if (DistEuclidea(c,cprima) < 100)
	{
		return (this->columnas[longitud(c)][latitud(c)]._adyacentes).Pertenece(cprima);
	}
	else
	{
		return this->hayCamino2(c,cprima);
	}

}

bool Mapa::hayCamino2 (const Coordenada& c , const Coordenada& cprima) const{

	Conj<Coordenada> coords = this->coords;
	return this->ExisteCamino(c,cprima,coords);

}

bool Mapa::ExisteCamino(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const{

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


bool Mapa::ExisteCaminoPorArriba(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const {

	
	
		if (cs.Pertenece(CoordenadaArriba(c)))
		{
			cs.Eliminar(CoordenadaArriba(c));
			return this->ExisteCamino(CoordenadaArriba(c),cprima,cs);
		}
		else
		{
		return false;
		}
	
	

}


bool Mapa::ExisteCaminoPorAbajo(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const {

	if (latitud(c)>0)
	{
		if (cs.Pertenece(CoordenadaAbajo(c)))
		{
			cs.Eliminar(CoordenadaAbajo(c));
			return this->ExisteCamino(CoordenadaAbajo(c),cprima,cs);

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}


bool Mapa::ExisteCaminoPorIzquierda(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const {

	if (longitud(c)>0)
	{
		if (cs.Pertenece(CoordenadaALaIzquierda(c)))
		{
			cs.Eliminar(CoordenadaALaIzquierda(c));
			return this->ExisteCamino(CoordenadaALaIzquierda(c),cprima,cs);

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}


}

bool Mapa::ExisteCaminoPorDerecha(const Coordenada& c, const Coordenada& cprima, Conj<Coordenada>& cs ) const {

	if (cs.Pertenece(CoordenadaALaDerecha(c)))
	{
		cs.Eliminar(CoordenadaALaDerecha(c));
		return this->ExisteCamino(CoordenadaALaDerecha(c),cprima,cs);

	}
	else
	{
		return false;
	}

}



std::ostream& operator<<(std::ostream& os, const Mapa& m){
	os <<"Coordenadas:" ;
	
	/*	
	for (int i = 0; i < this->columnas.Longitud; ++i)
	{
		if (this->columnas[i].Longitud() > max_Y )
		{
			max_Y = this->columnas[i].Longitud();
		}
	}

	for (int i = 0; i < max_X; ++i)
	{
		for (int j = 0; j < max_Y; ++i)
		{
			
		}
	}
	*/

	Conj<Coordenada>::const_Iterador it = m.coordenadas();
	while(it.HaySiguiente())
	{
		os << it.Siguiente();
		it.Avanzar();
	}

    return os;


}
