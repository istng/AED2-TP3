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
	Coordenada I = _jugadores[e].posicion;
	Coordenada F = c;
	Conj<Pokemon>::Iterador poke = (_pokemonsTotales.claves()).CrearIt();
	while(poke.HaySiguiente())
	{
		Conj<Coordenada>::Iterador posPoke = (_pokemonsTotales.Obtener(poke.Siguiente())).CrearIt();
		while(posPoke.HaySiguiente())
		{
			if (posPoke.Siguiente().SiguienteClave() == I)
			{
				LaCoordendaEsInicio(posPoke,I,F,e,j);
			}
			else if (posPoke.Siguiente().SiguienteClave() == F)
			{
				LaCoordendaEsFinal(posPoke,I,F,e,j);
			}
			else  
			{
				LaCoordendaEsFinal(posPoke,I,F,e,j);
			}

			// fijarse donde se avanza posPoke!
		}
		poke.Avanzar();
	}

	EliminarSiguiente(jugadores[e].posMatriz());
	_jugadore[e].posMatriz = _matrizJugadores[c.longitud][c.latitud].AgregarRapido(e);
	_jugadores[e].posicion = c;
	if ( DistEuclidea(I,F) >= 100 )
	{
		if (! mapa.hayCamino(I,F))
		{
			_jugadores[e].sanciones = _jugadores[e].sanciones + 1;
		}
	}
	
	if(_jugadores[e].sanciones >= 5 )
	{
		expulsarJugador(e);
	}
	
}

// Otras operaciones

const Conj<Coordenada> Juego::coordARadio(const Coordenada &c, Nat r) const{
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

bool Juego::pudoAgregarPokemon(const Coordenada& c) const{
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

bool Juego::hayPokemonCercano(const Coordenada& c) const{
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

const Coordenada& Juego::posPokemonCercano(const Coordenada& c) const{
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

Nat Juego::indiceDeRareza(const Pokemon& p) const{
   return 100 - (100*cantMismaEspecie(p))/cantidadPokeTotal;
}


// privado
Vector< Vector< Conj<Jugador> > >& Juego::crearMatrizJug(const Mapa& m) const
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

Vector< Vector<Juego::infoMatrizPoke> >& Juego::crearMatrizPokes(const Mapa& m) const
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
	HeapModificable* pokeHeap = new HeapModificable();
	Nat i = -2;
	while(i <= 2){
		Nat j = -2;
		while(j <= 2){
			Coordenada coorJug = Coordenada(c.longitud + i, c.latitud + j);
			if(_mapa.posExistente(coorJug) && DistEuclidea(c, coorJug) <= 2){
				Conj<Jugador>::Iterador itJugHeap = matrizJugadores[i][j].CrearIt();
				while(itJugHeap.HaySiguiente()){
					Nat id = itJugHeap.Siguiente();
					HeapModificable::Iterador itPokeHeap = pokeHeap->encolar(HeapModificable::JugadorHeap(cantidadPokemons(id), id));
					_jugadores[id].prioridad = itPokeHeap;
					itJugHeap.Avanzar();
				}
			}
			++j;
		}
		++i;
	}
	
	return *pokeHeap;
}

void Juego::laCoordenadaEsInicio(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e)
{
	Coordenada k = posPoke.Siguiente().SiguienteClave();
	if (hayPokemonCercano(k) && hayPokemonCercano(F,j))
	{
		if (posPokemonsCercano(k) !=  posPokemonsCercano(F))
		{
			posPoke.Siguiente().SiguienteSignificado().cantMovCapt = 0;
		}
	}
	//Aca avanzamos posPoke 
	Avanzar(posPoke);
	
}

void Juego::laCoordenadaEsFinal(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F, Jugador e)
{
	Coordenada k = posPoke.Siguiente().SiguienteClave();
	if (hayPokemonCercano(k) && hayPokemonCercano(I))
	{
		if (posPokemonsCercano(k) = posPokemonsCercano(I))
		{
			posPoke.Siguiente().SiguienteSignificado().cantMovCapt = 0;
			JugadorHeap jug = JugadorHeap(CantidadPokemons(e),e);
			HeapModificable* heap =  posPoke.Siguiente().SiguienteSignificado().colaPrioridad;
			_jugadores[e].prioridad.EliminarSiguiente();
			_jugadores[e].prioridad = &heap.encolar(jug);
		}
		if (!hayPokemonCercano(I))
		{
			posPoke.Siguiente().SiguienteSignificado().cantMovCapt = 0;
			JugadorHeap jug = JugadorHeap(CantidadPokemons(e),e);
			HeapModificable* heap =  posPoke.Siguiente().SiguienteSignificado().colaPrioridad;
			_jugadores[e].prioridad = &heap.encolar(jug);
		}
	}
	posPoke.Avanzar();
	
}

void Juego::laCoordenadaEsOtra(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke, Coordenada& I, Coordenada& F)
{
	Coordenada k = posPoke.Siguiente().SiguienteClave();
	if (hayPokemonCercano(k) && posPoke.Siguiente().SiguienteSignificado().cantMovCapt == 9)		
	{
		capturarPokemon(posPoke);
	}
	else if (hayPokemonCercano(k) && posPoke.Siguiente().SiguienteSignificado().cantMovCapt < 9)
	{
		posPoke.Siguiente().SiguienteSignificado().cantMovCapt = posPoke.Siguiente().SiguienteSignificado().cantMovCapt + 1;
	}

	posPoke.Avanzar();
	
}

void Juego::capturarPokemon(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador Poke)
{
	Coordenada k = poke.Siguiente().SiguienteClave();
	HeapModificable* posibles = poke.Siguiente().SiguienteSignificado().colaPrioridad;
	Pokemon tipo = poke.Siguiente().SiguienteSignificado().tipo;
	if (!posibles.esVacia())
	{
		Jugador jugGanador = posibles->proximo().id;
		darlePokemon(jugGanador,tipo);
		_matrizPokemons[k.longitud][k.latitud].hayPoke = false;
		//este iterador se invalida:
		//matrizPokemons[k.longitud][k.latitud].iterador
		poke.Siguiente().EliminarSiguiente();
		poke.EliminarSiguiente(); 
	}
	
}

void Juego::darlePokemon(Jugador e, const Pokemon& p)
{
	if (_jugadores[e].pokesRapido.Definido(p))
	{
		Conj<pokes>::Iterador iter = _jugadores[e].pokesRapido.Obtener(p);
		iter.Siguiente().cant = iter.Siguiente().cant + 1; 
	}
	else
	{
		pokes nuevoPokemon = pokes(p,1);
		Conj<pokes>::Iterador iter = _jugadores[e].pokemons.AgregarRapido(nuevoPokemon);
		_jugadore[e].pokesRapido.Definir(p,iter);
	}
	
}

void Juego::expulsarJugador(Jugador e)
{
	cantidadPo69keTotal = cantidadPokeTotal - cantidadPokemons(e);
	eliminarPokemons(e);
	_jugadores[e].posMatriz.EliminarSiguiente();
	if (_juagadores[e].prioridad.HaySiguiente())
	{
		_jugadores[e].prioridad.EliminarSiguiente();
	}
	_jugadores[e].conectado = false;
	Conj<Jugador>::Iterador expulsarJ = _juagadores[e].lugarNoExpulsado;
	//juagadores[e].lugarNoExpulsado = NULL
	expulsarJ.EliminarSiguiente();
	expulsados.AgregarRapido(e);	

	
}

Nat Juego::cantidadPokemons(Jugador e) const
{
	Nat res = 0;
	Conj<pokes>::const_Iterador iter = pokemons(e);
	while(iter.HaySiguiente()){
		res += iter.Siguiente().cant;
		iter.Avanzar();
	}
	return res;
}

void Juego::eliminarPokemons(Jugador e)
{
	Conj<Juego::pokes>::Iterador iter = pokemons(e);
	while(iter.HaySiguiente())
	{
		Pokemon tipo = iter.Siguiente().tipo;
		_pokemonsTotales.Obtener(pokemon).cant = _pokemonsTotales.Obtener(pokemon).cant - iter.Siguiente().cant;
		if (_pokemonsTotales.Obtener().cant == 0)
		{
			_pokemonsTotales.Borrar(pokemon);
			iter.EliminarSiguiente();

		}
	}
	
}

Nat Juego::cantPokemonsTotales() const
{
	return cantidadPokeTotal;
}

Nat Juego::cantMismaEspecie(const Pokemon& p) const {
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