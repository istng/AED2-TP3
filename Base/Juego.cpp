#include "Juego.h"


Juego(const Mapa& m)
{
	mapa = m;
	jugadoresNoExpulsados = Conj();
	jugadores = Vector();
	expulsados = Vector();
	matrizJugadores = crearMatrizJug(m);
	matrizPokemons = crearMatrizPokes(m);
	//el modulo tiene "pokemons" en este codigo. Se nos paso, lo sacamos de la estr... 
	posPokemons = Dicc();
	pokemonsTotales = DiccString();
	cantidadPokeTotal = 0;
}

void agregarPokemon(const Pokemon& p, const Coordenada& c)
{
	cantidadPokeTotal++;
	HeapModificable::Iterador itNuevoHeap = crearHeapPokemon(c); //creo que este aux no devuelve un it a heap en el .h
	
	infoCoord infoNueva;
		infoNueva.tipo = p;
		infoNueva.cantMovCapt = 0;
		infoNueva.colaPrioridad = itNuevoHeap;
	Dicc::Iterador dicLinealPoke = posPokemons.DefinirRapido(c,infoNueva);
	infoMatrizPoke infoMatNueva;
		infoMatNueva.hayPoke = true;
		infoMatNueva.iterador = dicLinealPoke;
	matrizPokemons[longitud(c)][latitud(c)] = infoMatrizPoke; //nose si estos son los metodos correctos de coord
	if (pokemonsTotales.Definido(p))
	{
		pokemonsTotales.Obtener(p).cant++;
		pokemonsTotales.Obtener(p).pos.AgregarRapido(dicLinealPoke);
	}
	else
	{
		infoPoke infoPokeNueva;
			infoPokeNueva.cant = 1;
			infoPokeNueva.pos = Conj();
			infoPokeNueva.pos.AgregarRapido(dicLinealPoke);
		pokemonsTotales.Definir(p,infoPokeNueva);
	}
}

Nat cantMismaEspecie(Pokemon p) const {
    DiccTrie<Pokemon, infoPoke>::const_Iterador it = pokemonsTotales.CrearIt();
    while(it.HaySiguiente()){
        if(it.SiguienteClave()==p){
            return it.SiguienteSignificado().cant;
        }
        it.Avanzar();
    }    
}

const Conj<Coordenada> coordARadio(Coordenada c,Nat r) const{
    //no se si no la entiendo, pero para mi esto hace cualquier cosa
    Conj<Coordenada> coords = Conj<Coordenada>();
    int i,j = 0;
    if(r <= c.latitud){
         i = c.latitud - r; 
    }
    if(r <= c.longitud){
         j = c.longitud - r;
    }
    while(i <= c.latitud + r){
         while(j<=c.longitud + r){
             if(posExistente(Coordenada(i,j),mapa)){
                 coords.AgregarRapido(Coordenada(i,j))
             }
             j++;
         }
         i++;
    }
    return coords;
}

bool hayPokemonCercano(Coordenada c) const{
    if(posExistente(c,mapa)){
        Conj<Coordenada> posibles = coordARadio(c,25);
        Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
        while(it.HaySiguiente()){
            if(matrizPokemons[it.Siguiente().longitud][it.Siguiente().longitud].hayPoke){
                return true;
            }
            it.Avanzar();
        }
    }
    return false;    
}

const Coordenada posPokemonsCercano(Coordenada c) const{
    if(posExistente(c,mapa)){
        Conj<Coordenada> posibles = coordARadio(c,25);
        Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
        while(it.HaySiguiente()){
            if(matrizPokemons[it.Siguiente().longitud][it.Siguiente().longitud].hayPoke){
                return it.Siguiente();
            }
            it.Avanzar();
        }
    }
}

const Conj<Jugador>& entrenadoresPosibles(const Coordenada& c) const{
    Conj<Jugador> entrenadores = Conj<Jugador>();
    Conj<Coordenada> posibles = coordARadio(c,25);
    Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
    while(it.HaySiguiente()){
        Conj<Jugador>::const_Iterador itJ = matrizJugadores[it.Siguiente().longitud][it.Siguiente().longitud]
        while(itJ.HaySiguiente()){
            entrenadores.AgregarRapido(itJ.Siguiente());
            itJ.Avanzar();
        }
        it.Avanzar();
    }
}

Nat indiceDeRareza(Pokemon p) const{
   return 100 - (100*cantMismaEspecie(p))/cantPokemonsTotales
}

bool puedoAgregarPokemon(Coordenada c) const{
   // if(posExistente(c,mapa)){
   //     Conj<Coordenada> posibles = coordARadio(c,25);
   //     Conj<Coordenada>::Iterador it = posibles.CrearIt();
   //     while(it.HaySiguiente()){
   //         if(matrizPokemons[it.Siguiente().longitud][it.Siguiente().longitud].hayPoke){
   //             return false;
   //         }
   //     }
   //     return true;
   // }
   // return false;
   return !hayPokemonCercano(c);    
}

Nat agregarJugador(Jugador j){
    Nat res = jugadores.Longitud();
    bool estaConectado = false;
    Nat sanciones = 0;
    Coordenada pos = Coordenada(42,42);
    Conj<pokes> pokes = Conj<pokes>();
    DiccString<Conj<pokes>::Iterador >  pokesRapido = DiccString<Conj<pokes> >();
    HeapModificable<Nat,Jugador> >::Iterador posCola = NULL;
    Conj<Jugador>::Iterador posMatriz = NULL;

    Conj<Jugador>::Iterador itNoExpulsado = jugadoresNoExpulsados.AgregarRapido(j);

    infoJug nuevo = infoJug(res, estaConecteado, sanciones, pos, pokes, pokesRapido, posCola, posMatriz, itNoExpulsado);
    jugadores.AgregarAtras(nuevo);
     
    return res;
}

void conectarse(Jugador j, Coordenada c){

    jugadores[j].estaConectado = true;
    jugadores[j].pos = c;
    Conj<Jugador>::Iterador itConj = matrizJugadores[c.latitud][c.longitud].AgregarRapido(j);
    jugadores[j].posMatriz = itConj;
    if(hayPokemonCercano(c)){
        Coordenada pokePosi = posPokemonsCercano(c);
        JugadorHeap nueva = JugadorHeap(CantidadPokemons(jugadores[j].pokemonsCapturados)j);
        HeapModificable::Iterador itHeap = matrizPokemons[pokePosi.longitud][pokePosi.latitud].colaPrioridad.encolar(nueva);
        jugadores[j].prioridad = itHeap;
        posPokemons.Significado(pokePosi).cantMovCapt = 0;
    }
}

void desconectarse(Jugador j){
    jugadores[j].estaConectado = false;
    jugadores[j].posMatriz.EliminarSiguiente();
    jugadores[j].posMatriz = NULL;
    Coordenada c = jugadores[j].pos;
    if(hayPokemonCercano(c)){
        Coordenada pokePosi = posPokemonsCercano(c);
        jugadores[j].prioridad.EliminarSiguiente();
        jugadores[j].prioridad = NULL;
        posPokemons.Significado(pokePosi).cantMovCapt = 0;
    }
}
