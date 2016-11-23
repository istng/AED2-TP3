// Compilar usando:
//	g++ -o tests tests.cpp Driver.cpp <lista de archivos *.cpp propios>
//	valgrind --leak-check=full ./tests

#include "Driver.h"
#include "mini_test.h"

using namespace aed2;

void test_constructor_con_mapa() {
  Conj<Coordenada> cc;

  cc.Agregar(Coordenada(0,0));
  cc.Agregar(Coordenada(0,1));
  cc.Agregar(Coordenada(0,2));
  cc.Agregar(Coordenada(1,2));
  cc.Agregar(Coordenada(10,0));
  cc.Agregar(Coordenada(1,4));

 


  Driver d(cc);
 // std::cout << "todo va bienaa" << std::endl;
  ASSERT( d.mapa() == cc );

 // ASSERT( true );
}

void test_agregar_jugadores(){
	Conj<Coordenada> cc;

  cc.Agregar(Coordenada(0,0));
  cc.Agregar(Coordenada(0,1));
  cc.Agregar(Coordenada(0,2));
  cc.Agregar(Coordenada(1,2));
  cc.Agregar(Coordenada(10,0));
  cc.Agregar(Coordenada(1,4));

  Driver d(cc);

  d.agregarPokemon("poke1", Coordenada(10,0));
  d.agregarPokemon("poke2", Coordenada(0,0));
  d.agregarPokemon("poke3", Coordenada(1,4));
  d.agregarPokemon("poke4", Coordenada(0,2));

  d.agregarJugador();
  ASSERT(! d.estaConectado(0));
  d.conectarse(0,Coordenada(0,0));
  ASSERT(d.posicion(0) == Coordenada(0,0));
 


}


void test_agregar_pokemones(){
  Conj<Coordenada> cc;

  cc.Agregar(Coordenada(0,0));
  cc.Agregar(Coordenada(0,1));
  cc.Agregar(Coordenada(0,2));
  cc.Agregar(Coordenada(1,2));
  cc.Agregar(Coordenada(10,0));
  cc.Agregar(Coordenada(1,4));

  Driver d(cc);

  d.agregarPokemon("poke1", Coordenada(10,0));
  d.agregarPokemon("poke2", Coordenada(0,0));
  d.agregarPokemon("poke3", Coordenada(1,4));
  d.agregarPokemon("poke4", Coordenada(0,2));

  d.agregarJugador();
  d.conectarse(0,Coordenada(0,0));
  

  ASSERT(d.pokemonEnPos(Coordenada(10,0)) == "poke1");
  ASSERT(d.pokemonEnPos(Coordenada(0,0)) == "poke2");
  ASSERT(d.pokemonEnPos(Coordenada(1,4)) == "poke3");
  ASSERT(d.pokemonEnPos(Coordenada(0,2)) == "poke4");
}

void test_puedo_agregar_pokemones(){
  Conj<Coordenada> cc;

  cc.Agregar(Coordenada(0,0));
  cc.Agregar(Coordenada(0,1));
  cc.Agregar(Coordenada(0,2));
  cc.Agregar(Coordenada(1,2));
  cc.Agregar(Coordenada(10,0));
  cc.Agregar(Coordenada(1,4));

  Driver d(cc);

  d.agregarPokemon("poke1", Coordenada(0,0));

  Nat distancia = 5;
  
  ASSERT(!d.puedoAgregarPokemon(Coordenada(10,4)));
  ASSERT(!d.puedoAgregarPokemon(Coordenada(0,0)));
  ASSERT(!d.puedoAgregarPokemon(Coordenada(200,100)));
  ASSERT(!d.puedoAgregarPokemon(Coordenada(1,4)));
  ASSERT(d.puedoAgregarPokemon(Coordenada(10,0)));
}


int main(int argc, char **argv)
{
   RUN_TEST(test_constructor_con_mapa);
 RUN_TEST(test_agregar_jugadores);
 RUN_TEST(test_agregar_pokemones);
 RUN_TEST(test_puedo_agregar_pokemones);
  
  return 0;
}
