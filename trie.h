#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include "resultado.h"
#include "pelicula.h"

using namespace std;

//Estructura para el manejo de ranking de los resultados
// [TESTEO P3: No finalizado aun]


// [TESTEO P2: No finalizado aun]
struct NodoTrie {
    NodoTrie* hijos[36]; // 26 letras + 10 números
    bool esFin;
    // Mapa: <idPelicula, frecuencia de la palabra en esa peli>
    map<int, int> contadorPeliculas;

    NodoTrie();
};

// [TESTEO P2: No finalizado aun]
class Trie {
private:
    NodoTrie* raiz;
    int obtenerIndice(char c);

    void recolectarResultados(NodoTrie* nodo, unordered_map<int, int>& resultadosAcumulados);

public:
    Trie();
    void insertar(const string& palabra, int idPelicula);
    void construirTrie(const vector<Pelicula>& peliculas);
    NodoTrie* getRaiz() {
        return raiz;
    }

    vector<Resultado> buscar(string consulta);
    vector<Resultado> obtenerPagina(const vector<Resultado>& todosLosResultados, int pagina, int tamanoPagina = 5);
    vector<Resultado> buscarFrase(const vector<string>& consulta);
    vector<Resultado> buscarPalabra(const string& consulta);

};
#endif//
