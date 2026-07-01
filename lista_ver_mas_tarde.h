#ifndef LISTA_VER_MAS_TARDE_H
#define LISTA_VER_MAS_TARDE_H

#include <vector>
#include "pelicula.h"

void agregarVerMasTarde(int idPelicula);
void agregarLike(int idPelicula);
void cargarListas();
void guardarListas();
void mostrarVerMasTarde(const std::vector<Pelicula>& peliculas);

#endif