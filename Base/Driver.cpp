#include "Driver.h"

// Instanciar un mapa y un this->juego 

Driver::Driver(const Conj< Coordenada > & cs) : juego(Juego(DameMapa(cs)))
{
  /*Mapa m;
  Conj< Coordenada >::const_Iterador coords = cs.CrearIt();
  while (coords.HaySiguiente())
  {
  	m.agregarCoor(coords.Siguiente());
  	coords.Avanzar();	
  }

  this->juego(m);*/
}

Driver::~Driver()
{
  assert(false);
}

void Driver::agregarPokemon(const Pokemon & p, const Coordenada & c)
{
  this->juego.agregarPokemon(p,c);
}


Jugador Driver::agregarJugador(){

	Jugador id = this->juego.agregarJugador();
	return id;
}

void Driver::conectarse(const Jugador & j, const Coordenada & c){

	this->juego.conectarse(j,c);
}

void Driver::desconectarse(const Jugador & j){
	this->juego.desconectarse(j);
}

void Driver::moverse(const Jugador & j, const Coordenada & c){
	this->juego.moverse(j,c);
}

Conj< Coordenada > Driver::mapa() const{

	Conj< Coordenada > res;
	Conj<Coordenada>::const_Iterador it = this->juego.mapa().coordenadas();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;
}

bool Driver::hayCamino(const Coordenada & c1, const Coordenada & c2) const{

	return this->juego.mapa().hayCamino(c1,c2);
}

bool Driver::posExistente(const Coordenada & c) const{

	return this->juego.mapa().posExistente(c);
}

Conj< Jugador > Driver::jugadores() const{

	Conj< Jugador > res;
	Conj< Jugador >::const_Iterador it = this->juego.jugadores();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;

}

bool Driver::estaConectado(const Jugador & j) const{
	return this->juego.estaConectado(j);
}


Nat Driver::sanciones(const Jugador & j) const{
	return this->juego.sanciones(j);
}

Coordenada Driver::posicion(const Jugador & j) const{
	return this->juego.posicion(j);
}

Dicc< Pokemon , Nat > Driver::pokemons(const Jugador & j) const{
	Dicc <Pokemon , Nat > res;
	Conj<Juego::pokes>::const_Iterador it = this->juego.pokemons(j);
	while (it.HaySiguiente())
	{
		res.Definir(it.Siguiente().tipo,it.Siguiente().cant);
	}
	return res;
}

Conj< Jugador > Driver::expulsados() const{

	Conj< Jugador > res;
	Conj< Jugador >::const_Iterador it = this->juego.expulsados();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;
}

Conj< Coordenada > Driver::posConPokemons() const{

	Conj< Coordenada > res;
	Conj<Coordenada>::const_Iterador it = this->juego.posConPokemons();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;

}

Pokemon Driver::pokemonEnPos(const Coordenada & c) const{
	return this->juego.pokemonEnPos(c);
}


Nat Driver::cantMovimientosParaCaptura(const Coordenada & c) const{
	return this->juego.cantMovimientosParaCapturar(c);
}


bool Driver::puedoAgregarPokemon(const Coordenada & c) const{
	return this->juego.pudoAgregarPokemon(c);
}


bool Driver::hayPokemonCercano(const Coordenada & c) const{
	return this->juego.hayPokemonCercano(c);
}

Coordenada Driver::posPokemonCercano(const Coordenada & c) const{
	return this->juego.posPokemonCercano(c);
}


Conj<Jugador> Driver::entrenadoresPosibles(const Coordenada & c) const{
	return this->juego.entrenadoresPosibles(c);
}

Nat Driver::indiceRareza(const Pokemon & p) const{
	return this->juego.indiceDeRareza(p);
}


Mapa Driver::DameMapa(const Conj<Coordenada>& cs) const {

	Mapa m;
  	Conj< Coordenada >::const_Iterador coords = cs.CrearIt();
 	while (coords.HaySiguiente())
  	{
  		m.agregarCoor(coords.Siguiente());
  		coords.Avanzar();	
  	}
  	return m;
} 



// TODO: Completar con el resto de las implementaciones