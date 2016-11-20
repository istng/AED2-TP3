#ifndef JUEGO_H
#define JUEGO_H

#include "TiposJuego.h" 
#include "aed2/TiposBasicos.h"
#include "aed2/Vector.h"
#include "aed2/Conj.h"
#include "aed2/Dicc.h"
#include "DiccString.h"
#include "Mapa.h"
#include "HeapModificable.h"

using namespace aed2;

class Juego
{
	public:
	
		// No se puede crear el tipo matriz con typedef Vector<Vector<T> > Matriz<T>
		// usamos Vector<Vector<T> > directamente
		// typedef pokemonsCapturados Nat; // no lo usamos¿¿??
		
		struct pokes;
	
	// Observadores
		const Mapa& mapa() const; 
		Conj<Jugador>::const_Iterador jugadores() const; 
		bool estaConectado(Jugador e) const;
		Nat sanciones(Jugador e) const;
		const Coordenada& posicion(Jugador e) const;
		Conj<pokes>::const_Iterador pokemons(Jugador e) const; // cambie multiconjunto/tupla por pokes
		const Conj<Jugador>::const_Iterador expulsados() const; 
		const Conj<Coordenada>::const_Iterador posConPokemons() const;
		const Pokemon& pokemonEnPos(const Coordenada& c) const;
		Nat cantMovimientosParaCapturar(const Coordenada& c) const;
		
	// Generadores 
		Juego(const Mapa& m); // crearJuego renombrado como constructor
		void agregarPokemon(const Pokemon& p, const Coordenada& c);
		Nat agregarJugador();
		void conectarse(Jugador j, const Coordenada& c);
		void desconectarse(Jugador j);
		void moverse(Jugador e,const Coordenada& c);
		
	// Otras operaciones
        const Conj<Coordenada> coordARadio(const Coordenada &c, Nat r) const;
		bool pudoAgregarPokemon(const Coordenada &c) const;
		bool hayPokemonCercano(const Coordenada &c) const;
		const Coordenada& posPokemonCercano(const Coordenada &c) const;
		const Conj<Jugador>& entrenadoresPosibles(const Coordenada& c) const;
		Nat indiceDeRareza(const Pokemon &p) const;
		
	// pokes es necesario que sea publico porqur pokemons devuelve un Conj de pokes
		struct pokes{
			Pokemon tipo;
			Nat cant;
            pokes(Pokemon p , Nat cant): tipo(p),cant(cant){};
		};
		
	private:
	
		struct infoMatrizPoke;
		struct infoJug;
		struct infoCoord;
		struct infoPoke;
	
		Mapa _mapa;
		Vector<infoJug> _jugadores; // cambie el nombre porque choca con la funcion jugadores
		Conj<Jugador> jugadoresNoExpulsados;
		Conj<Jugador> _expulsados; // cambie el nombre porque choca con la funcion expulsados
		Vector< Vector< Conj<Jugador> > > matrizJugadores;
		Vector< Vector<infoMatrizPoke> > matrizPokemons;
		Dicc<Coordenada, infoCoord> posPokemons;
		DiccString<infoPoke> pokemonsTotales;
		Nat cantidadPokeTotal;
		
		struct infoMatrizPoke{
			infoMatrizPoke() : hayPoke(false), iterador(Dicc<Coordenada, infoCoord>::Iterador()) {};
			bool hayPoke;
            Dicc<Coordenada, infoCoord>::Iterador iterador;
		};
		
		struct infoJug{
			bool conectado;
			Nat sanciones;
			Coordenada posicion;
			Conj<Juego::pokes> pokemons; //ConjuntoLineal == Conj ??
			DiccString<Conj<Juego::pokes>::Iterador > pokesRapido;
            HeapModificable::Iterador prioridad; // HeapModificable no es una clase generica
            Conj<Jugador>::Iterador posMatriz;
            Conj<Jugador>::Iterador lugarNoExpulsado;
			
            infoJug(bool c, Nat s, Coordenada p, Conj<pokes> pokes, DiccString<Conj<Juego::pokes>::Iterador > pr, HeapModificable::Iterador prior, Conj<Jugador>::Iterador pMatriz, Conj<Jugador>::Iterador lugarNoExp) 
				: conectado(c), sanciones(s), posicion(p), pokemons(pokes), pokesRapido(pr), prioridad(prior), posMatriz(pMatriz), lugarNoExpulsado(lugarNoExp) {};
        };
	
		struct infoCoord{
			Pokemon tipo;
			Nat cantMovCapt;
			HeapModificable colaPrioridad;
		};
		
		struct infoPoke{
			Nat cant;
            Conj< Dicc<Coordenada, infoCoord>::Iterador > pos;
		};
		
		Vector<Vector<Conj<Jugador> > >& crearMatrizJug(const Mapa& m) const;
		Vector< Vector<infoMatrizPoke> >& crearMatrizPokes(const Mapa& m) const;
		HeapModificable& crearHeapPokemon(const Coordenada& c);
		void laCoordenadaEsInicio(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e);
		void laCoordenadaEsFinal(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e);
		void laCoordenadaEsOtra(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F);
		void capturarPokemon(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador Poke);
		void darlePokemon(Jugador e, const Pokemon& p);
		void expulsarJugador(Jugador e);
		Nat cantidadPokemons(Jugador e) const;
		void eliminarPokemons(Jugador e);
		Nat cantMismaEspecie(const Pokemon& p) const;
		Nat cantPokemonsTotales() const;
};

#endif
