#ifndef PREPROCESADOR_H
#define PREPROCESADOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "pelicula.h"
#include "Resultado.h"

using namespace std;

extern unordered_set<string> stopwords;

string normalizar(const string& texto);

// Template
template <typename Contenedor = vector<string>>
Contenedor tokenizar(const string& texto)
{
    Contenedor palabras;
    string actual;

    for(char c : texto)
    {
        if(c == ' ')
        {
            if(!actual.empty())
            {
                if(stopwords.count(actual) == 0)
                {
                    palabras.insert(palabras.end(), actual);
                }
                actual.clear();
            }
        }
        else
        {
            actual += c;
        }
    }

    if(!actual.empty())
    {
        if(stopwords.count(actual) == 0)
        {
            palabras.insert(palabras.end(), actual);
        }
    }

    return palabras;
}


unordered_map<string, vector<int>> crearIndice(
    const vector<Pelicula>& peliculas,
    const string& campo
);

vector<Resultado> buscarPorTag(
    const unordered_map<string, vector<int>>& indice,
    const string& consulta
);

#endif