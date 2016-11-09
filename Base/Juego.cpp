#include "Juego.h"

Nat AgregarJugador(Jugador j){

    Nat res = jugadores.Longitud();
    bool estaConectado = false;
    Nat sanciones = 0;
    Coordenada pos = Coordenada(42,42);
    Conj<pokes> pokes = Conj<pokes>();
    DiccString<Iterador::Conj<pokes> >  pokesRapido = DiccString<Iterador::Conj<pokes> >();
    Iterador::HeapModificable<Nat,Jugador> > posCola = NULL;
    Iterador::Conj<Jugador> posMatriz = NULL;

    Iterador::Conj<Jugador> itNoExpulsado = jugadoresNoExpulsados.AgregarRapido(j);

    infoJug nuevo = infoJug(res, estaConecteado, sanciones, pos, pokes, pokesRapido, posCola, posMatriz, itNoExpulsado);
    jugadores.AgregarAtras(nuevo);
    
    return res;
}

void Conectarse(Jugador j, Coordenada c){

    jugadores[j].estaConectado = true;
    jugadores[j].pos = c;
    Iterador::Conj<>


    


}
