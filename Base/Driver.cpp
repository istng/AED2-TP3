#include "Driver.h"

// Instanciar un mapa y un this->juego 

Driver::Driver(const Conj< Coordenada > & cs)
{
  this->pMapa =  new Mapa;
  Conj< Coordenada >::const_Iterador coords = cs.CrearIt();
  while (coords.HaySiguiente())
  {
    pMapa->agregarCoor(coords.Siguiente());
  	coords.Avanzar();	
  }

  this->pJuego = new Juego(*pMapa);
}

Driver::~Driver(){
    delete this->pJuego;
    delete this->pMapa;
}

void Driver::agregarPokemon(const Pokemon & p, const Coordenada & c)
{
  this->pJuego->agregarPokemon(p,c);
}


Jugador Driver::agregarJugador(){

	Jugador id = this->pJuego->agregarJugador();
	return id;
}

void Driver::conectarse(const Jugador & j, const Coordenada & c){

	this->pJuego->conectarse(j,c);
}

void Driver::desconectarse(const Jugador & j){
	this->pJuego->desconectarse(j);
}

void Driver::moverse(const Jugador & j, const Coordenada & c){
	this->pJuego->moverse(j,c);
}

Conj<Coordenada> Driver::mapa() const{

	Conj<Coordenada> res;
	Conj<Coordenada>::const_Iterador it = this->pJuego->mapa().coordenadas();
    while (it.HaySiguiente())
	{
        std::cout << "coordenada:" << it.Siguiente() << std::endl;
		res.Agregar(it.Siguiente());
		it.Avanzar();
	}

    return res;
}

bool Driver::hayCamino(const Coordenada & c1, const Coordenada & c2) const{

	return this->pJuego->mapa().hayCamino(c1,c2);
}

bool Driver::posExistente(const Coordenada & c) const{

	return this->pJuego->mapa().posExistente(c);
}

Conj< Jugador > Driver::jugadores() const{

	Conj< Jugador > res;
	Conj< Jugador >::const_Iterador it = this->pJuego->jugadores();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;

}

bool Driver::estaConectado(const Jugador & j) const{
	return this->pJuego->estaConectado(j);
}


Nat Driver::sanciones(const Jugador & j) const{
	return this->pJuego->sanciones(j);
}

Coordenada Driver::posicion(const Jugador & j) const{
	return this->pJuego->posicion(j);
}

Dicc< Pokemon , Nat > Driver::pokemons(const Jugador & j) const{
	Dicc <Pokemon , Nat > res;
	Conj<Juego::pokes>::const_Iterador it = this->pJuego->pokemons(j);
	while (it.HaySiguiente())
	{
		res.Definir(it.Siguiente().tipo,it.Siguiente().cant);
	}
	return res;
}

Conj< Jugador > Driver::expulsados() const{

	Conj< Jugador > res;
	Conj< Jugador >::const_Iterador it = this->pJuego->expulsados();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;
}

Conj< Coordenada > Driver::posConPokemons() const{

	Conj< Coordenada > res;
	Conj<Coordenada>::const_Iterador it = this->pJuego->posConPokemons();
	while (it.HaySiguiente())
	{
		res.Agregar(it.Siguiente());
	}

	return res;

}

Pokemon Driver::pokemonEnPos(const Coordenada & c) const{
	return this->pJuego->pokemonEnPos(c);
}


Nat Driver::cantMovimientosParaCaptura(const Coordenada & c) const{
	return this->pJuego->cantMovimientosParaCapturar(c);
}


bool Driver::puedoAgregarPokemon(const Coordenada & c) const{
	return this->pJuego->pudoAgregarPokemon(c);
}


bool Driver::hayPokemonCercano(const Coordenada & c) const{
	return this->pJuego->hayPokemonCercano(c);
}

Coordenada Driver::posPokemonCercano(const Coordenada & c) const{
	return this->pJuego->posPokemonCercano(c);
}


Conj<Jugador> Driver::entrenadoresPosibles(const Coordenada & c) const{
	return this->pJuego->entrenadoresPosibles(c);
}

Nat Driver::indiceRareza(const Pokemon & p) const{
	return this->pJuego->indiceDeRareza(p);
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
