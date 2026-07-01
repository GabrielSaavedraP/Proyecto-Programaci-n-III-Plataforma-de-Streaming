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
#include "resultado.h"
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
    
    Trie();                    
    int obtenerIndice(char c);
    void insertar(const string& palabra, int idPelicula);
    void recolectarResultados(NodoTrie* nodo, unordered_map<int, int>& resultadosAcumulados);

public:
    ~Trie();
    static Trie& getInstance();
    
    void construirTrie(const vector<Pelicula>& peliculas);
    
    vector<Resultado> buscar(string consulta);
    vector<Resultado> buscarPalabra(const string& consulta);
    vector<Resultado> buscarFrase(const vector<string>& palabras);
    vector<Resultado> obtenerPagina(const vector<Resultado>& todosLosResultados, int pagina, int tamanoPagina = 5);
    
    NodoTrie* getRaiz() { return raiz; }
};

#endif