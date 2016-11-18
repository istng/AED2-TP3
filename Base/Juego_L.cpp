#ifndef TP3_JUEGO_CPP
#define TP3_JUEGO_CPP

#include "Juego.h"

using namespace aed2;

// Observadores
const Mapa& Juego::mapa() const
{
	return _mapa;
}

Conj<Jugador>::const_Iterador Juego::jugadores() const
{
	return jugadoresNoExpulsados.CrearIt();
}

bool Juego::estaConectado(Jugador e) const
{
	return _jugadores[e].conectado;
}

Nat Juego::sanciones(Jugador e) const
{
	return _jugadores[e].sanciones; 
}

const Coordenada& Juego::posicion(Jugador e) const
{
	return _jugadores[e].posicion;
}

Conj<Juego::pokes>::const_Iterador Juego::pokemons(Jugador e) const
{
	return _jugadores[e].pokemons.CrearIt();
}

const Conj<Jugador>::const_Iterador Juego::expulsados() const 
{
	return _expulsados.CrearIt();
}

const Conj<Coordenada>::const_Iterador Juego::posConPokemons() const
{
    // DICC NO TIENE NINGUNA FUNCION QUE TE DE LAS CLAVES!!!!
	return Conj<Coordenada>::const_Iterador();
}

const Pokemon& Juego::pokemonEnPos(const Coordenada& c) const
{
	Dicc<Coordenada, infoCoord>::Iterador it = matrizPokemons[longitud(c)][latitud(c)].iterador;
	return it.SiguienteSignificado().tipo;
}

Nat Juego::cantMovimientosParaCapturar(const Coordenada& c) const
{
	Dicc<Coordenada, infoCoord>::Iterador it = matrizPokemons[longitud(c)][latitud(c)].iterador;
	return it.SiguienteSignificado().cantMovCapt;
}


// Gerneradores
Juego::Juego(const Mapa& m)
{
	_mapa = m;
	jugadoresNoExpulsados = Conj<Jugador>();
	_jugadores = Vector<infoJug>();
	_expulsados = Conj<Jugador>();
	matrizJugadores = crearMatrizJug(m);
	matrizPokemons = crearMatrizPokes(m);
	//el modulo tiene "pokemons" en este codigo. Se nos paso, lo sacamos de la estr... 
	posPokemons = Dicc<Coordenada, infoCoord>();
	pokemonsTotales =DiccString<infoPoke>();
	cantidadPokeTotal = 0;
}

void Juego::agregarPokemon(const Pokemon& p, const Coordenada& c)
{
	cantidadPokeTotal++;
	//HeapModificable::Iterador itNuevoHeap = crearHeapPokemon(c); //creo que este aux no devuelve un it a heap en el .h
	
	infoCoord infoNueva;
		infoNueva.tipo = p;
		infoNueva.cantMovCapt = 0;
		infoNueva.colaPrioridad = crearHeapPokemon(c); // no es un iterador
	Dicc<Coordenada, infoCoord>::Iterador dicLinealPoke = posPokemons.DefinirRapido(c,infoNueva);
	infoMatrizPoke infoMatNueva;
		infoMatNueva.hayPoke = true;
		infoMatNueva.iterador = dicLinealPoke;
	matrizPokemons[longitud(c)][latitud(c)] = infoMatNueva; //nose si estos son los metodos correctos de coord // cambié infoMatrizPoke por infoMatNueva
	if (pokemonsTotales.Definido(p))
	{
		pokemonsTotales.Obtener(p).cant++;
		pokemonsTotales.Obtener(p).pos.AgregarRapido(dicLinealPoke);
	}
	else
	{
		infoPoke infoPokeNueva;
			infoPokeNueva.cant = 1;
			infoPokeNueva.pos = Conj< Dicc<Coordenada, infoCoord>::Iterador >();
			infoPokeNueva.pos.AgregarRapido(dicLinealPoke);
		pokemonsTotales.Definir(p,infoPokeNueva);
	}
}

Nat Juego::agregarJugador(){
    Nat res = _jugadores.Longitud();
    
    bool estaConectado = false;
    Nat sanciones = 0;
    Coordenada pos = Coordenada(42,42);
    Conj<Juego::pokes> pokes = Conj<Juego::pokes>();
    DiccString< Conj<Juego::pokes>::Iterador > pokesRapido = DiccString<Conj<Juego::pokes>::Iterador >();
    HeapModificable::Iterador posCola = HeapModificable::Iterador(NULL,NULL);
    Conj<Jugador>::Iterador posMatriz = Conj<Jugador>::Iterador();

    Conj<Jugador>::Iterador itNoExpulsado = jugadoresNoExpulsados.AgregarRapido(res);

    infoJug nuevo = infoJug(estaConectado, sanciones, pos, pokes, pokesRapido, posCola, posMatriz, itNoExpulsado);
    
   
    _jugadores.AgregarAtras(nuevo);
    
    
    return res;
}

void Juego::conectarse(Jugador j, const Coordenada& c){

    _jugadores[j].conectado = true;
    _jugadores[j].posicion = c;
    Conj<Jugador>::Iterador itConj = matrizJugadores[c.latitud][c.longitud].AgregarRapido(j);
    _jugadores[j].posMatriz = itConj;
    if(hayPokemonCercano(c)){
        Coordenada pokePosi = posPokemonCercano(c);
        HeapModificable::JugadorHeap nueva = HeapModificable::JugadorHeap(cantidadPokemons(j), j);
        HeapModificable::Iterador itHeap = matrizPokemons[pokePosi.longitud][pokePosi.latitud].iterador.SiguienteSignificado().colaPrioridad.encolar(nueva);
        _jugadores[j].prioridad = itHeap;
        posPokemons.Significado(pokePosi).cantMovCapt = 0;
    }
}

void Juego::desconectarse(Jugador j){
    _jugadores[j].conectado = false;
    _jugadores[j].posMatriz.EliminarSiguiente();
    _jugadores[j].posMatriz = Conj<Jugador>::Iterador(); // No existe "iterador NULL"
    Coordenada c = _jugadores[j].posicion;
    if(hayPokemonCercano(c)){
        Coordenada pokePosi = posPokemonCercano(c);
        _jugadores[j].prioridad.eliminarSiguiente();
        _jugadores[j].prioridad = HeapModificable::Iterador(NULL,NULL); // No existe "iterador NULL"
        posPokemons.Significado(pokePosi).cantMovCapt = 0;
    }
}

void Juego::moverse(Jugador e, const Coordenada& c)
{
	
}

// Otras operaciones

const Conj<Coordenada> Juego::coordARadio(Coordenada c, Nat r) const{
    //no se si no la entiendo, pero para mi esto hace cualquier cosa
    Conj<Coordenada> coords = Conj<Coordenada>();
    Nat i,j = 0;
    if(r <= c.latitud){
         i = c.latitud - r; 
    }
    if(r <= c.longitud){
         j = c.longitud - r;
    }
    while(i <= c.latitud + r){
         while(j<=c.longitud + r){
             if(_mapa.posExistente(Coordenada(i,j))){
                 coords.AgregarRapido(Coordenada(i,j));
             }
             j++;
         }
         i++;
    }
    return coords;
}

bool Juego::pudoAgregarPokemon(Coordenada c) const{
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

bool Juego::hayPokemonCercano(Coordenada c) const{
    if(_mapa.posExistente(c)){
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

const Coordenada& Juego::posPokemonCercano(Coordenada c) const{
    if(_mapa.posExistente(c)){
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

const Conj<Jugador>& Juego::entrenadoresPosibles(const Coordenada& c) const{
    Conj<Jugador>* entrenadores = new Conj<Jugador>();
    Conj<Coordenada> posibles = coordARadio(c,25);
    Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
    while(it.HaySiguiente()){
        Conj<Jugador>::const_Iterador itJ = matrizJugadores[it.Siguiente().longitud][it.Siguiente().longitud].CrearIt();
        while(itJ.HaySiguiente()){
            entrenadores->AgregarRapido(itJ.Siguiente());
            itJ.Avanzar();
        }
        it.Avanzar();
    }
	return *entrenadores;
}

Nat Juego::indiceDeRareza(Pokemon p) const{
   return 100 - (100*cantMismaEspecie(p))/cantidadPokeTotal;
}


// privado
Vector< Vector< Conj<Jugador> > >& Juego::crearMatrizJug(const Mapa& m)
{
	Vector< Vector< Conj<Jugador> > >* res = new Vector< Vector< Conj<Jugador> > >();
	Conj<Coordenada>::const_Iterador it = m.coordenadas();
	
	while(it.HaySiguiente()){
		Coordenada c = it.Siguiente();
		if(res->Longitud() < c.longitud){
			Nat i = res->Longitud();
			while(i <= c.longitud){
				res->AgregarAtras(Vector< Conj<Jugador> >());
				++i;
			}
		}
		// Acá cambio bastante!!!
		for(Nat i = 0; i < res->Longitud(); ++i){
			if((*res)[i].Longitud() < c.latitud){
				for(Nat j = (*res)[i].Longitud(); j < c.latitud; ++j){
					(*res)[j].AgregarAtras(Conj<Jugador>());
				}
			}
		}
		it.Avanzar();
	}
	return *res;
}

Vector< Vector<Juego::infoMatrizPoke> >& Juego::crearMatrizPokes(const Mapa& m)
{
	Vector< Vector<infoMatrizPoke> >* res = new Vector< Vector<infoMatrizPoke> >();
	Conj<Coordenada>::const_Iterador it = m.coordenadas();
	
	while(it.HaySiguiente()){
		Coordenada c = it.Siguiente();
		if(res->Longitud() < c.longitud){
			Nat i = res->Longitud();
			while(i <= c.longitud){
				res->AgregarAtras(Vector<infoMatrizPoke>());
				++i;
			}
		}
		// Acá cambio bastante!!!
		for(Nat i = 0; i < res->Longitud(); ++i){
			if((*res)[i].Longitud() < c.latitud){
				for(Nat j = (*res)[i].Longitud(); j < c.latitud; ++j){
					(*res)[j].AgregarAtras(infoMatrizPoke());
				}
			}
		}
		it.Avanzar();
	}
	return *res;
}

HeapModificable& Juego::crearHeapPokemon(const Coordenada& c)
{
	HeapModificable* hm = new HeapModificable();
	return *hm;
}

void Juego::laCoordenadaEsInicio(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e)
{
	
}

void Juego::laCoordenadaEsFinal(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e)
{
	
}

void Juego::laCoordenadaEsOtra(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F)
{
	
}

void Juego::capturarPokemon(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador Poke)
{
	
}

void Juego::darlePokemon(Jugador e, Pokemon& p)
{
	
}

void Juego::expulsarJugador(Jugador e)
{
	
}

Nat Juego::cantidadPokemons(Jugador e) const
{
	return 0;
}

void Juego::eliminarPokemons(Jugador e)
{
	
}

Nat Juego::cantPokemonsTotales() const
{
	return cantidadPokeTotal;
}

Nat Juego::cantMismaEspecie(Pokemon p) const {
	/*
    DiccString<infoPoke>::const_Iterador it = pokemonsTotales.CrearIt();
    while(it.HaySiguiente()){
        if(it.SiguienteClave()==p){
            return it.SiguienteSignificado().cant;
        }
        it.Avanzar();
    }   
	*/
	//DiccString no tiene iterador, y mejor usar obtener directamente
	return pokemonsTotales.Obtener(p).cant;
}

#endif