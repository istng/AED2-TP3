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
