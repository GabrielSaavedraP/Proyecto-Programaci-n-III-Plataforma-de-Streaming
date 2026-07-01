#ifndef PELICULA_BUILDER_H
#define PELICULA_BUILDER_H

#include "pelicula.h"
#include <string>
using namespace std;

class PeliculaBuilder {
private:
    Pelicula pelicula;

public:
    PeliculaBuilder& setReleaseYear(const string& valor) {
        pelicula.releaseYear = valor;
        return *this;
    }
    PeliculaBuilder& setTitulo(const string& valor) {
        pelicula.titulo = valor;
        return *this;
    }
    PeliculaBuilder& setOrigen(const string& valor) {
        pelicula.origen = valor;
        return *this;
    }
    PeliculaBuilder& setDirector(const string& valor) {
        pelicula.director = valor;
        return *this;
    }
    PeliculaBuilder& setCast(const string& valor) {
        pelicula.cast = valor;
        return *this;
    }
    PeliculaBuilder& setGenero(const string& valor) {
        pelicula.genero = valor;
        return *this;
    }
    PeliculaBuilder& setUrl(const string& valor) {
        pelicula.url = valor;
        return *this;
    }
    PeliculaBuilder& setSinopsis(const string& valor) {
        pelicula.sinopsis = valor;
        return *this;
    }
    Pelicula build() {
        return pelicula;
    }
};

#endif