#include "Juego.h"

using namespace aed2;

// Observadores
const Mapa& Juego::mapa() const
{
	return *this->_mapa;
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

const Conj<Coordenada>& Juego::posConPokemons() const
{
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


Juego::Juego(const Mapa& m): _mapa(&m),jugadoresNoExpulsados(),_jugadores(),_expulsados(),matrizJugadores(crearMatrizJug(m)),matrizPokemons(crearMatrizPokes(m)),posPokemons(),pokemonsTotales(new DiccString<infoPoke>()),cantidadPokeTotal(0){}
/*{
	_mapa = &m;
	jugadoresNoExpulsados = Conj<Jugador>();
	_jugadores = Vector<infoJug>();
	_expulsados = Conj<Jugador>();
	matrizJugadores = crearMatrizJug(m);
	matrizPokemons = crearMatrizPokes(m);
	//el modulo tiene "pokemons" en este codigo. Se nos paso, lo sacamos de la estr... 
	posPokemons = Dicc<Coordenada, infoCoord>();
	pokemonsTotales = new DiccString<infoPoke>();
	cantidadPokeTotal = 0;
}*/

Juego::~Juego(){ 

	delete this->pokemonsTotales;

 }

void Juego::agregarPokemon(const Pokemon& p, const Coordenada& c)
{
	cantidadPokeTotal++;
	//HeapModificable::Iterador itNuevoHeap = crearHeapPokemon(c); //creo que este aux no devuelve un it a heap en el .h
	
	infoCoord infoNueva;
		infoNueva.tipo = p;
		infoNueva.cantMovCapt = 0;
		infoNueva.colaPrioridad = &crearHeapPokemon(c); // no es un iterador
	Dicc<Coordenada, infoCoord>::Iterador dicLinealPoke = posPokemons.DefinirRapido(c,infoNueva);
	infoMatrizPoke infoMatNueva;
		infoMatNueva.hayPoke = true;
		infoMatNueva.iterador = dicLinealPoke;
	matrizPokemons[longitud(c)][latitud(c)] = infoMatNueva; //nose si estos son los metodos correctos de coord // cambié infoMatrizPoke por infoMatNueva
	if (pokemonsTotales->Definido(p))
	{
		pokemonsTotales->Obtener(p).cant++;
		pokemonsTotales->Obtener(p).pos.AgregarRapido(dicLinealPoke);
	}
	else
	{
		infoPoke infoPokeNueva;
			infoPokeNueva.cant = 1;
			infoPokeNueva.pos = Conj< Dicc<Coordenada, infoCoord>::Iterador >();
			infoPokeNueva.pos.AgregarRapido(dicLinealPoke);
		pokemonsTotales->Definir(p,infoPokeNueva);
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
    Conj<Jugador>::Iterador itConj = matrizJugadores[c.longitud][c.latitud].AgregarRapido(j);
    _jugadores[j].posMatriz = itConj;
    if(hayPokemonCercano(c)){
        Coordenada pokePosi = posPokemonCercano(c);
        HeapModificable::JugadorHeap nueva = HeapModificable::JugadorHeap(cantidadPokemons(j), j);


        HeapModificable::Iterador itHeap = matrizPokemons[pokePosi.longitud][pokePosi.latitud].iterador.SiguienteSignificado().colaPrioridad->encolar(nueva);
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
	Conj<Pokemon>::Iterador poke = (pokemonsTotales->Claves()).CrearIt();
	while(poke.HaySiguiente())
	{
		Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador posPoke = pokemonsTotales->Obtener(poke.Siguiente()).pos.CrearIt();
		while(posPoke.HaySiguiente())
        {
            bool seMovioDesde = DistEuclidea(posPoke.Siguiente().SiguienteClave(),I) <= 4;
            bool seMovioHacia = DistEuclidea(posPoke.Siguiente().SiguienteClave(),F) <= 4;

            // Para el caso 1 no entra pasa todas las guardas deveria evaluar a false pues seMovioDesde y SeMovioHacia son true.

            if (seMovioDesde && !seMovioHacia)  //Caso 2
			{
                laCoordenadaEsInicio(posPoke,I,F,e);
			}
            else if (!seMovioDesde && seMovioHacia )  //Caso 3
			{
				laCoordenadaEsFinal(posPoke,I,F,e);
			}
            else if( !seMovioDesde && !seMovioHacia)  //Caso 4
			{
				Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador it2 = posPoke;
				it2.Avanzar();
				laCoordenadaEsOtra(posPoke,I,F);
				posPoke = it2;
			}

			// fijarse donde se avanza posPoke!
		}
		poke.Avanzar();
	}

	_jugadores[e].posMatriz.EliminarSiguiente();
	_jugadores[e].posMatriz = matrizJugadores[c.longitud][c.latitud].AgregarRapido(e);
	_jugadores[e].posicion = c;
	if ( DistEuclidea(I,F) >= 100 )
	{
		if (! _mapa->hayCamino(I,F))
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
    Conj<Coordenada> coords;
    Nat i = 0;
    Nat j = 0;
    if(r <= c.latitud){
         i = c.latitud - r; 
    }
    if(r <= c.longitud){
         j = c.longitud - r;
    }
    while(i <= c.latitud + r){
    	if(r <= c.longitud){
        	j = c.longitud - r;
    	}
    	else j = 0;
         while(j <= c.longitud + r){
         	 
         	 Nat i1 = c.latitud - i;
         	 Nat j1 = c.longitud - j;

         	 if (i1< 0){
         	 	i1 = r - c.latitud;
         	 }
         	 if (j1 < 0){
         	 	j1 = r - c.longitud;
         	 }

             if(_mapa->posExistente(Coordenada(i,j)) && ( i1 * i1 + j1 * j1 <= r * r )){
             	//std::cout<< "j: " << j << "  i:" << i <<  "   *****************"<< std::endl;
                 coords.AgregarRapido(Coordenada(i,j));
             }
             j++;

         }
        // std::cout<< "i:" << i << std::endl;
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
   if(_mapa->posExistente(c)){
   	 //std::cout<< "ssssssssssssssss"<< std::endl;
   	   return !hayPokemonCercano(c);  
   }
   return false;
}

bool Juego::hayPokemonCercano(const Coordenada& c) const{
    if(_mapa->posExistente(c)){
        Conj<Coordenada> posibles = coordARadio(c,5);
        Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
        while(it.HaySiguiente()){
            if(matrizPokemons[it.Siguiente().longitud][it.Siguiente().latitud].hayPoke){
                return true;
            }
            it.Avanzar();
        }
    }
    return false;    
}

const Coordenada Juego::posPokemonCercano(const Coordenada& c) const{
    if(_mapa->posExistente(c)){
        Conj<Coordenada> posibles = coordARadio(c,5);
        Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
        while(it.HaySiguiente()){
            if(matrizPokemons[it.Siguiente().longitud][it.Siguiente().latitud].hayPoke){
                return it.Siguiente();
            }
            it.Avanzar();
        }

    }
}

const Conj<Jugador> Juego::entrenadoresPosibles(const Coordenada& c) const{
    Conj<Jugador> entrenadores = Conj<Jugador>();
    Conj<Coordenada> posibles = coordARadio(c,2);
    Conj<Coordenada>::const_Iterador it = posibles.CrearIt();
    while(it.HaySiguiente()){
        Conj<Jugador>::const_Iterador itJ = matrizJugadores[it.Siguiente().longitud][it.Siguiente().latitud].CrearIt();
        while(itJ.HaySiguiente()){
            entrenadores.AgregarRapido(itJ.Siguiente());
            itJ.Avanzar();
        }
        it.Avanzar();
    }
	return entrenadores;
}

Nat Juego::indiceDeRareza(const Pokemon& p) const{
   return 100 - (100*cantMismaEspecie(p))/cantidadPokeTotal;
}


// privado
Vector< Vector< Conj<Jugador> > > Juego::crearMatrizJug(const Mapa& m) const
{


	Nat longitudMax = 0;
	Nat latitudMax = 0;

	Conj<Coordenada>::const_Iterador it = m.coordenadas();

	// Buscamos la latidud maxima y la longitud maxima

	while(it.HaySiguiente()){
		if(it.Siguiente().longitud > longitudMax) longitudMax = it.Siguiente().longitud;
		if(it.Siguiente().latitud > latitudMax) latitudMax = it.Siguiente().latitud;
		it.Avanzar();
	}

	// Creamos la matriz

	Vector< Vector<Conj<Jugador > > > res = Vector< Vector<Conj<Jugador> > >();

	for(Nat i = 0; i <= longitudMax; ++i){
		res.AgregarAtras(Vector<Conj<Jugador > >());
		for(Nat j = 0; j <= latitudMax; ++j){
			res[i].AgregarAtras(Conj<Jugador>());
		}
	}

	return res;

	
}

Vector< Vector<Juego::infoMatrizPoke> > Juego::crearMatrizPokes(const Mapa& m) const
{
	// La idea es crear una matriz rectangular que contenga todas las coordenadas de mapa
	// y cada celda de la matriz contiene un infoMatrizPoke sin informacion

	Nat longitudMax = 0;
	Nat latitudMax = 0;

	Conj<Coordenada>::const_Iterador it = m.coordenadas();

	// Buscamos la latidud maxima y la longitud maxima

	while(it.HaySiguiente()){
		if(it.Siguiente().longitud > longitudMax) longitudMax = it.Siguiente().longitud;
		if(it.Siguiente().latitud > latitudMax) latitudMax = it.Siguiente().latitud;
		it.Avanzar();
	}

	// Creamos la matriz

	Vector< Vector<infoMatrizPoke> > res = Vector< Vector<infoMatrizPoke> >();

	for(Nat i = 0; i <= longitudMax; ++i){
		res.AgregarAtras(Vector<infoMatrizPoke>());
		for(Nat j = 0; j <= latitudMax; ++j){
			res[i].AgregarAtras(infoMatrizPoke());
		}
	}

	return res;

	/*
	Vector< Vector<infoMatrizPoke> >* res = new Vector< Vector<infoMatrizPoke> >();
	Conj<Coordenada>::const_Iterador it = m.coordenadas();
	
	while(it.HaySiguiente()){
		Coordenada c = it.Siguiente();
		if(res->Longitud() < c.longitud + 1){
			Nat i = res->Longitud();
			while(i <= c.longitud){
				res->AgregarAtras(Vector<infoMatrizPoke>());
				++i;
			}
		}
		// Acá cambio bastante!!!
		for(Nat i = 0; i < res->Longitud(); ++i){
			if((*res)[i].Longitud() < c.latitud + 1){
				for(Nat j = (*res)[i].Longitud(); j < c.latitud + 1; ++j){
					(*res)[i].AgregarAtras(infoMatrizPoke());
				}
			}
		}
		it.Avanzar();
	}

    
	return *res;
*/
}

HeapModificable& Juego::crearHeapPokemon(const Coordenada& c)
{
	HeapModificable* pokeHeap = new HeapModificable();
		Conj<Coordenada> coords = coordARadio(c,2);
		Conj<Coordenada>::const_Iterador it = coords.CrearIt();
		while(it.HaySiguiente())
		{
			Nat i = it.Siguiente().longitud;
			Nat j = it.Siguiente().latitud;
			Conj<Jugador>::Iterador itJugHeap = matrizJugadores[i][j].CrearIt();
			while(itJugHeap.HaySiguiente()){
				Nat id = itJugHeap.Siguiente();
				HeapModificable::Iterador itPokeHeap = pokeHeap->encolar(HeapModificable::JugadorHeap(cantidadPokemons(id), id));
				_jugadores[id].prioridad = itPokeHeap;
				itJugHeap.Avanzar();
			}
			it.Avanzar();
		
		}
	
	return *pokeHeap;
}


/*
 * I <-- coordenada de inicio.
 * F <-- coordenada destino (final).
 *
 *
 *   Para el generador mover existen los siguientes casos:
 *
 *   1) El jugador se mueve dentro del rango en el que estaba.    <=>     I y F estan tienen el mismo poke cercano.
 *   2) El jugador se mueve desde fuera de un rango hacia uno.    <=>     I no tiene poke cercano y F si.
 *   3) El jugador se mueve desde dentro de un rango hacia fuera. <=>     I tiene poke cerano y F no.
 *   4) El jugador se mueve desde un rango hacia otro.            <=>     I y F tienen poke cercano pero no es el mismo.
 *   5) El jugador no se mueve ni desde ni hacia un rango.        <=>     I y F no tienen poke cercano.
 *
 *
 *   //// Esto tendria que ser lo mismo que:
 *
 *  Para cada coordenada que contenga un pokemon.
 *
 *  1) El jugador se mueve dentro de su rango.         <=> la coordenada esta en rango de I y F.
 *  2) El jugador se mueve desde su rango a fuera.     <=> la coordenada esta en rango de I y no de F.
 *  3) El jugador se mueve hacia su rango desde fuera. <=> la coordenada esta en rango F y no de I.
 *  4) El jugador se mueve desde su rango hacia.       <=> la coordeanad no esa en rango de F ni de I.
 *
 *  Notar que cubrimos todos los casos de I y F para una coordenda.Ahora dada una coordenada, lo que hariamos para cada uno de estos seria lo siguiente:
 *
 *  1) No hacemos nada.
 *
 *  2) *) lo sacamos del heap.
 *     *) reiniciamos el contador.
 *
 *  3) *)lo encolamos al heap.
 *     *) reiniciamos el contador.
 *
 *  4) si el cantMovCapt tiene 9:
 *       *) se captura el pokemon.
 *     si no:
 *       *) se aumenta en 1 el contador.
 *
 *
 *
 *
*/


// Caso 2 ::::: la coord es la inicial y no es la final.

void Juego::laCoordenadaEsInicio(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador& posPoke, Coordenada& I, Coordenada& F, Jugador e)
{
	Coordenada k = posPoke.Siguiente().SiguienteClave();

    Dicc<Coordenada, Juego::infoCoord>::Iterador it = posPoke.Siguiente();
    it.SiguienteSignificado().cantMovCapt = 0; // Reiniciamos en contador cantMovCapt del pokemon de la coordenada de inicio
    _jugadores[e].prioridad.eliminarSiguiente(); // Sacamos al jugador de la cola de prioridad del pokemon , se invalida el it


	//Aca avanzamos posPoke 
	posPoke.Avanzar();
	
}


// Caso 3 ::::: la coord es la final y no es la inicial.

void Juego::laCoordenadaEsFinal(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador& posPoke, Coordenada& I, Coordenada& F, Jugador e)
{
	Coordenada k = posPoke.Siguiente().SiguienteClave();

    Dicc<Coordenada, Juego::infoCoord>::Iterador it = posPoke.Siguiente();
    it.SiguienteSignificado().cantMovCapt = 0; // el jugador se metio , reiniciamos el contador
    HeapModificable::JugadorHeap jug = HeapModificable::JugadorHeap(cantidadPokemons(e),e);
    HeapModificable* heap =  (it.SiguienteSignificado().colaPrioridad);
    _jugadores[e].prioridad = heap->encolar(jug); // lo metemos en el heap de pokemon cuya pos esta en rango del Jug

	posPoke.Avanzar();
	
}

// Caso 4 ::::: la coord no es inicial ni final.


void Juego::laCoordenadaEsOtra(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador& posPoke, Coordenada& I, Coordenada& F)
{

	Coordenada k = posPoke.Siguiente().SiguienteClave();
	Dicc<Coordenada, Juego::infoCoord>::Iterador it = posPoke.Siguiente();
	HeapModificable* posibles = it.SiguienteSignificado().colaPrioridad;
	if (it.SiguienteSignificado().cantMovCapt == 9 && !posibles->esVacia() )		
	{
		capturarPokemon(posPoke);
	}
	else if ( it.SiguienteSignificado().cantMovCapt < 9 )
	{
		it.SiguienteSignificado().cantMovCapt = it.SiguienteSignificado().cantMovCapt + 1;
	}

	posPoke.Avanzar();
	
}

void Juego::capturarPokemon(Conj< Dicc<Coordenada, infoCoord>::Iterador >::Iterador Poke)
{
	Coordenada k = Poke.Siguiente().SiguienteClave();
	Dicc<Coordenada, Juego::infoCoord>::Iterador it = Poke.Siguiente();
	HeapModificable* posibles = it.SiguienteSignificado().colaPrioridad;
	Pokemon tipo = it.SiguienteSignificado().tipo;
	
	Jugador jugGanador = posibles->proximo().id;
	darlePokemon(jugGanador,tipo);
	matrizPokemons[k.longitud][k.latitud].hayPoke = false;
	//este iterador se invalida:
	//matrizPokemons[k.longitud][k.latitud].iterador
	it.EliminarSiguiente();
	Poke.EliminarSiguiente();
	
	
}

void Juego::darlePokemon(Jugador e, const Pokemon& p)
{
	if (_jugadores[e].pokesRapido.Definido(p))
	{
		Conj<pokes>::Iterador iter = _jugadores[e].pokesRapido.Obtener(p);
		Nat cant = iter.Siguiente().cant + 1 ;
		//iter.Siguiente().cant = iter.Siguiente().cant + 1; 
		iter.EliminarSiguiente();
		pokes nuevo = pokes(p,cant);
		Conj<pokes>::Iterador nuevo_iter = _jugadores[e].pokemons.AgregarRapido(nuevo);
		_jugadores[e].pokesRapido.Definir(p,nuevo_iter);
	}
	else
	{
		pokes nuevoPokemon = pokes(p,1);
		Conj<pokes>::Iterador iter = _jugadores[e].pokemons.AgregarRapido(nuevoPokemon);
		_jugadores[e].pokesRapido.Definir(p,iter);
	}
	
}

void Juego::expulsarJugador(Jugador e)
{
	cantidadPokeTotal = cantidadPokeTotal - cantidadPokemons(e);
	eliminarPokemons(e);
	_jugadores[e].posMatriz.EliminarSiguiente();
	if (_jugadores[e].prioridad.haySiguiente())
	{
		_jugadores[e].prioridad.eliminarSiguiente();
	}
	_jugadores[e].conectado = false;
	Conj<Jugador>::Iterador expulsarJ = _jugadores[e].lugarNoExpulsado;
	Conj<Jugador> a;
	_jugadores[e].lugarNoExpulsado = a.CrearIt();
	expulsarJ.EliminarSiguiente();
	_expulsados.AgregarRapido(e);	

	
}

Nat Juego::cantidadPokemons(Jugador e) const
{
	Nat res = 0;
	Conj<Juego::pokes>::const_Iterador iter = _jugadores[e].pokemons.CrearIt();
	while(iter.HaySiguiente()){
		res += iter.Siguiente().cant;
		iter.Avanzar();
	}
	return res;
}

void Juego::eliminarPokemons(Jugador e)
{
	Conj<Juego::pokes>::Iterador iter = _jugadores[e].pokemons.CrearIt();
	while(iter.HaySiguiente())
	{
		Pokemon pokemon = iter.Siguiente().tipo;
		pokemonsTotales->Obtener(pokemon).cant = pokemonsTotales->Obtener(pokemon).cant - iter.Siguiente().cant;
		if (pokemonsTotales->Obtener(pokemon).cant == 0)
		{
			pokemonsTotales->Borrar(pokemon);
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
    DiccString<infoPoke>::const_Iterador it = pokemonsTotales->CrearIt();
    while(it.HaySiguiente()){
        if(it.SiguienteClave()==p){
            return it.SiguienteSignificado().cant;
        }
        it.Avanzar();
    }   
	*/
	//DiccString no tiene iterador, y mejor usar obtener directamente
	return pokemonsTotales->Obtener(p).cant;
}



