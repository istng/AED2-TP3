#ifndef JUEGO_H_
#define JUEGO_H_

#include "TiposJuego.h"
#include "mapa.hpp"
class Juego{

    public:
        
        Juego(Mapa m);

        ~Juego();

        void AgregarPokemon(Pokemon p, Coordenada c);

        void AgregarJugador(Jugador j);

        void Conectarse(Jugador j, Coordenada c);

        void Desconectarse(Jugador j);
        
        void Moverse(Jugador j, Coordenada c);

        Mapa Mapa();

        //iTConj::Conj<Jugadores> Jugadores();

        bool EstaConectado(Jugador j);

        Nat Sanciones(Jugador j);

        Coordenada Posicion(Jugador j);

        //itConj::Conj<Pokemon,Nat> Pokemons(Jugador j);

        Conj<Jugadores>::Expulsados();

        Conj<Pokemon> PosConPokemon();

        Pokemon PokemonEnPos(Coordenada c);

        Nat CantMovimientosParaCaptura(Coordenada c);
        
        bool PuedoAgregarPokemon(Coordenada c);
        
        bool HayPokemonCercano(Coordenada c);

        Coordenada PosPokemonCercano(Coordenada c);

        Conj<Jugadores> EntrenadoresPosibles(Coordenada c, Conjunto<Jugadores> js);

        Nat IndiceDeRareza(Pokemon p);
}
