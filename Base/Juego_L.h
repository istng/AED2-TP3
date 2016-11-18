#ifndef TP3_JUEGO_
#define TP3_JUEGO_

#include "TiposJuego.h" 
#include "aed2/TiposBasicos.h"
#include "aed2/Vector.h"
#include "aed2/Conj.h"
#include "aed2/Dicc.h"
#include "DiccString.hpp"
#include "mapa.hpp"
#include "HeapModificable.h"

using namespace aed2;

class Juego
{
	public:
	
		//typedef pokemonsCapturados Nat; // solo aparece en heap
		
		//template<class T>
		//typedef Vector<Vector<T>> Matriz<T>;
		
		struct pokes;
	
	// Observadores
		const Mapa& mapa() const; 
		Conj<Jugador>::const_Iterador jugadores() const; 
		bool estaConectado(Jugador e) const;
		Nat sanciones(Jugador e) const;
		const Coordenada& posicion(Jugador e) const;
		Conj<pokes>::const_Iterador pokemons(Jugador e) const; // cambi� multiconjunto/tupla por pokes
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
		void moverse(Jugador e, const Coordenada& c);
		
	// Otras operaciones
        const Conj<Coordenada> coordARadio(Coordenada c, Nat r) const;
		bool pudoAgregarPokemon(Coordenada c) const;
		bool hayPokemonCercano(Coordenada c) const;
		const Coordenada& posPokemonCercano(Coordenada c) const;
		const Conj<Jugador>& entrenadoresPosibles(const Coordenada& c) const;
		Nat indiceDeRareza(Pokemon p) const;
		
	// pokes
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
		Vector<infoJug> _jugadores; // cambi� el nombre porque choca con la funci�n jugadores
		Conj<Jugador> jugadoresNoExpulsados;
		Conj<Jugador> _expulsados; // cambi� el nombre porque choca con la funci�n expulsados
		Vector< Vector< Conj<Jugador> > > matrizJugadores;
		//Matriz< Conj<Jugador> > matrizJugadores;
		Vector< Vector<infoMatrizPoke> > matrizPokemons;
		//Matriz<infoMatrizPoke> matrizPokemons;
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
			DiccString<Conj<Juego::pokes> > pokesRapido;
            HeapModificable::Iterador prioridad; // HeapModificable no es una clase gen�rica
            Conj<Jugador>::Iterador posMatriz;
            Conj<Jugador>::Iterador lugarNoExpulsado;
			
		/*	infoJug(Jugador e, Conj<Jugador>::Iterador itNoExpulsado)
			 : conectado(false), sanciones(0), posicion(Coordenada(42,42)), pokemons(Conj<pokes>()), 
			 pokesRapido(DiccString<Conj<Juego::pokes> >()), prioridad(HeapModificable::Iterador(NULL,NULL)),
			 posMatriz(Conj<Jugador>::Iterador()), lugarNoExpulsado(itNoExpulsado)
			 {};
			*/
            infoJug(bool c, Nat s, Coordenada p, Conj<pokes> pokes, DiccString<Conj<Juego::pokes> > pr, HeapModificable::Iterador prior, Conj<Jugador>::Iterador pMatriz, Conj<Jugador>::Iterador lugarNoExp) 
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
		
		Vector<Vector<Conj<Jugador> > >& crearMatrizJug(const Mapa& m);
		Vector< Vector<infoMatrizPoke> >& crearMatrizPokes(const Mapa& m);
		HeapModificable& crearHeapPokemon(const Coordenada& c);
		void laCoordenadaEsInicio(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e);
		void laCoordenadaEsFinal(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e);
		void laCoordenadaEsOtra(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F);
		void capturarPokemon(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador Poke);
		void darlePokemon(Jugador e, Pokemon& p);
		void expulsarJugador(Jugador e);
		Nat cantidadPokemons(Jugador e) const;
		void eliminarPokemons(Jugador e);
		Nat cantMismaEspecie(Pokemon p) const;
		Nat cantPokemonsTotales(Pokemon p) const;
};

#endif
