#ifndef LISTA_VER_MAS_TARDE_H
#define LISTA_VER_MAS_TARDE_H

#include <vector>
#include "pelicula.h"

using namespace std;

extern vector<int> listaVerMasTarde;
extern vector<int> listaLikes;


void agregarVerMasTarde(int idPelicula);
void agregarLike(int idPelicula);
void cargarListas();
void guardarListas();
void mostrarVerMasTarde(const vector<Pelicula>& peliculas);

#endif