#ifndef TP3_JUEGO_
#define TP3_JUEGO_

#include "TiposJuego.h"

class Juego
{
	public:
	
		typedef pokemonsCapturados Nat; // solo aparece en heap
		typedef Vector< Vector<T> > Matriz<T>;
	
	// Observadores
		const Mapa& mapa() const; 
		Conj<Jugador>::const_Iterador jugadores() const;
		bool estaConectada(Jugador e) const;
		Nat sanciones(Jugador e) const;
		const Coordenada& posicion(Jugador e) const;
		Conj<Pokemon, Nat>::const_Iterador pokemons(Jugador e) const;
		const Conj<Jugador>& expulsados() const;
		const Conj<Coordenada>& posConPokemons() const;
		const Pokemon& pokemonEnPos(const Coordenada& c) const;
		Nat cantMovimientosParaCapturar(const Coordenada& c) const;
		
	// Generadores 
		Juego(const Mapa& m); // crearJuego renombrado como constructor
		void agregarPokemon(const Pokemon& p, const Coordenada& c);
		void agrgarJugador();
		void conectarse(Jugador e, const Coordenada& c);
		void desconectarse(Jugador e);
		void moverse(Jugador e, const Coordenada& c);
		
	// Otras operaciones
		bool pudoAgregarPokemon(const Coordenada& c);
		bool hayPokemonCercano(const Coordenada& c) const;
		const Coordenada& posPokemonCercano(const Coordenada& c) const;
		const Conj<Jugador>& entrenadoresPosibles(const Coordenada& c, const Conj<Jugador>& j) const;
		Nat indiceDeRareza(const Pokemon& p) const;
		
	private:
	
		struct infoMatrizPoke;
		struct infoJug;
		struct pokes;
		struct infoCoord;
		struct infoPoke;
		
		Mapa mapa;
		Vector<infoJug> jugadores;
		Conj<Jugador> jugadoresNoExpulsados;
		Conj<Jugador> expulsados;
		Matriz< Conj<Jugador> > matrizJugadores;
		Matriz<infoMatrizPoke> matrizPokemons;
		Dicc<Coordenada, infoCoord> posPokemons;
		DiccTrie<Pokemon, infoPoke> pokemonsTotales;
		Nat cantidadPokeTotal;
		
		struct infoMatrizPoke{
			bool hayPoke;
			Dicc<Coordenada, infoCoord>::Iterador iterador;
		}
		
		struct infoJug{
			bool conectado;
			Nat sanciones;
			Coordenada posicion;
			Conj<pokes> pokemons; //ConjuntoLineal == Conj ??
			DiccTrie<Pokemon, Conj<pokes> >::Iterador> pokesRapido;
			HeapModificable::Iterador prioridad; // HeapModificable no es una clase gen�rica
			Conj<Jugador>::Iterador posMatriz;
			Conj<Jugador>::Iterador lugarNoExpulsado;
		}
	
		struct pokes{
			Pokemon tipo;
			Nat cant;
		}
		
		struct infoCoord{
			Pokemon tipo;
			Nat cantMovCapt;
			HeapModificable colaPrioridad;
		}
		
		struct infoPoke{
			Nat cant;
			Conj< Dicc<Coordenada, infoCoord>::Iterador > pos;
		}
		
		Matriz< Conj<Jugador> >& crearMatrizJug(const Mapa& m);
		Matriz<infoMatrizPoke>& crearMatrizPokes(const Mapa& m);
		HeapModificable& crearHeapPokemon(const Coordenada& c);
		void laCoordenadaEsInicio(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e);
		void laCoordenadaEsFinal(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e);
		void laCoordenadaEsOtra(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F);
		void capturarPokemon(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador Poke);
		void darlePokemon(Jugador e, Pokemon& p);
		void expulsarJugador(Jugador e);
		Nat cantidadPokemons(Jugador e) const;
		void eliminarPokemons(Jugador e);
		Nat cantMismaEspecie(const Pokemon& p) const;
		Nat cantPokemonsTotales(const Pokemon& p) const;
}

#endif