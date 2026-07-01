#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>

#include "estrategias.h"
#include "Resultado.h"
#include "pelicula.h"
#include "preprocesador.h"

using namespace std;

struct NodoTrie {
    NodoTrie* hijos[36];
    bool esFin;
    map<int, int> contadorPeliculas;
    NodoTrie();
    ~NodoTrie();
};

class Trie {
private:
    static Trie* instancia;
    static std::mutex mutexInstancia;
    
    NodoTrie* raiz;
    EstrategiaRanking* estrategiaActual;
    
    Trie();                    
    int obtenerIndice(char c);
    void insertar(const string& palabra, int idPelicula);
    void recolectarResultados(NodoTrie* nodo, unordered_map<int, int>& resultadosAcumulados);

public:
    ~Trie();
    static Trie& getInstance();
    
    void construirTrie(const vector<Pelicula>& peliculas);

    void setEstrategia(EstrategiaRanking* nuevaEstrategia);
    
    vector<Resultado> buscar(string consulta, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes);
    vector<Resultado> buscarPalabra(const string& consulta, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes);
    vector<Resultado> buscarFrase(const vector<string>& palabras, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes);
    vector<Resultado> obtenerPagina(const vector<Resultado>& todosLosResultados, int pagina, int tamanoPagina = 5);
    
    NodoTrie* getRaiz() { return raiz; }
};

#endif