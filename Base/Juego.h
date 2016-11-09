#ifndef JUEGO_H_
#define JUEGO_H_

#include "TiposJuego.h"
#include "mapa.hpp"
#include "HeapModificable.h"
struct pokes{

    Pokemon poke;
    Nat cant;

}

template<class T> 
typedef Vector<Vector<T> > Matriz; 

class Juego{

    public:
        
        Juego(Mapa& m);

        ~Juego();

        void AgregarPokemon(Pokemon p, Coordenada c);

        void AgregarJugador(Jugador j);

        void Conectarse(Jugador j, Coordenada c);

        void Desconectarse(Jugador j);
        
        void Moverse(Jugador j, Coordenada c);

        Mapa Mapa() const;

        const_Iterador::Conj<Jugadores> Jugadores() const;

        bool EstaConectado(Jugador j) const;

        Nat Sanciones(Jugador j) const;

        Coordenada Posicion(Jugador j) const;

        const_Iterador::Conj<tupla> Pokemons(Jugador j) const;

        Conj<Jugadores>::Expulsados() const;

        Conj<Coordenada> PosConPokemon() const;

        Pokemon PokemonEnPos(Coordenada c) const;

        Nat CantMovimientosParaCaptura(Coordenada c) const;
        
        bool PuedoAgregarPokemon(Coordenada c) const;
        
        bool HayPokemonCercano(Coordenada c) const;

        Coordenada PosPokemonCercano(Coordenada c) const;

        Conj<Jugadores> EntrenadoresPosibles(Coordenada c, Conjunto<Jugadores> js) const;

        Nat IndiceDeRareza(Pokemon p) const;

        friend ostream& operator<<(ostream& os, const Juego& j){
            j.mostrar(os);
            return os;
        }

    private:
        
        struct infoJug{

            bool conectado;

            Nat sanciones;

            Coordenada posicion;

            Conj<pokes> pokemons;

            DiccString<Iterador::Conj<pokes> > pokesRapido;

            //Iterador::HeapModificable<Nat, Jugador> prioridad;
            
            Iterador::Conj<Jugador> posMatriz;
          
            Iterador::Conj<Jugador> lugarNoExpulsado;

        }

        struct infoMatrizPoke{
            
            bool hayPoke;

            Iterador::Dicc<Coordenada, infoCoord> iterador;

        }

        struct infoCoord{
            
            Pokemon tipo;

            Nat cantMovCap;

            //HeapModificable<Nat, Jugador> colaDePrioridad;

        }

        struct infoPoke{
        
            Nat cant;

            Conj<Iterrador::Dicc<Coordenada, infoCoor> >

        }

        Mapa mapa;
        
        Vector<infoJug> jugadores;
 
        Conj<Jugadores> jugadoresNoExpulsados;

        Conj<Jugadores> expulsados;

        Matriz<Conj<Jugadores> > matrizJugadores;        

        Matriz<infoMatrizPoke> matrizPokemons;

        Dicc<Coordenada, infoCoordenada> posPokemons;

        DiccString<infoPoke> pokemonesTotales;

        Nat cantidadPokeTotal;

}

#endif // JUEGO_H_
