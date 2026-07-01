#include "trie.h"
#include <iostream>
#include <algorithm>
#include <chrono>      // Para medicion de tiempos
#include <thread>      // Para paralelizacion
#include <mutex>       // Para seguridad en hilos

using namespace std;

//Singleton
Trie* Trie::instancia = nullptr;
std::mutex Trie::mutexInstancia;

Trie::Trie() {
    raiz = new NodoTrie();
}

Trie::~Trie() {
    delete raiz;
}

Trie& Trie::getInstance() {
    std::lock_guard<std::mutex> lock(mutexInstancia);
    if (instancia == nullptr) {
        instancia = new Trie();
    }
    return *instancia;
}

// Nodotrie
NodoTrie::NodoTrie() {
    for (int i = 0; i < 36; i++) hijos[i] = nullptr;
    esFin = false;
}

NodoTrie::~NodoTrie() {
    for (int i = 0; i < 36; i++) {
        delete hijos[i];
    }
}

// Metodos internos (igual que el proy 1)
int Trie::obtenerIndice(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= '0' && c <= '9') return c - '0' + 26;
    return -1;
}

void Trie::insertar(const string& palabra, int idPelicula) {
    NodoTrie* actual = raiz;
    for (char c : palabra) {
        int indice = obtenerIndice(c);
        if (indice == -1) continue;
        if (actual->hijos[indice] == nullptr) {
            actual->hijos[indice] = new NodoTrie();
        }
        actual = actual->hijos[indice];
    }
    actual->esFin = true;
    actual->contadorPeliculas[idPelicula]++;
}

// Busquedas (integrante 1)
vector<Resultado> Trie::buscarPalabra(const string& consulta) {
    string limpio = normalizar(consulta);
    NodoTrie* actual = raiz;
    for (char c : limpio) {
        int indice = obtenerIndice(c);
        if (indice == -1) continue;
        if (actual->hijos[indice] == nullptr) return {};
        actual = actual->hijos[indice];
    }

    unordered_map<int, int> consolidado;
    recolectarResultados(actual, consolidado);

    priority_queue<Resultado> pq;
    for (auto const& [id, frec] : consolidado) {
        pq.push({id, frec});
    }

    vector<Resultado> lista;
    while (!pq.empty()) {
        lista.push_back(pq.top());
        pq.pop();
    }
    return lista;
}

vector<Resultado> Trie::buscar(string consulta) {
    string limpio = normalizar(consulta);
    vector<string> palabras = tokenizar(limpio);
    if (palabras.empty()) return {};
    if (palabras.size() == 1)
        return buscarPalabra(palabras[0]);
    return buscarFrase(palabras);
}

void Trie::recolectarResultados(NodoTrie *nodo, unordered_map<int, int> &resultadosAcumulados) {
    if (nodo == nullptr) return;
    for (auto const& [id, frec] : nodo->contadorPeliculas) {
        resultadosAcumulados[id] += frec;
    }
    for (int i = 0; i < 36; i++) {
        if (nodo->hijos[i] != nullptr) {
            recolectarResultados(nodo->hijos[i], resultadosAcumulados);
        }
    }
}

vector<Resultado> Trie::buscarFrase(const vector<string>& palabras) {
    unordered_map<int,int> consolidado;
    for(const string& palabra : palabras) {
        vector<Resultado> resultados = buscarPalabra(palabra);
        for(const auto& r : resultados) {
            consolidado[r.idPelicula] += r.relevancia;
        }
    }
    priority_queue<Resultado> pq;
    for(const auto& [id,frec] : consolidado) {
        pq.push({id,frec});
    }
    vector<Resultado> lista;
    while(!pq.empty()) {
        lista.push_back(pq.top());
        pq.pop();
    }
    return lista;
}

// Construccion paralela
void Trie::construirTrie(const vector<Pelicula>& peliculas) {
    if (peliculas.empty()) return;

    cout << "[Trie] Iniciando construcción del índice...\n";

    // Versión Paralela 
    auto startPar = std::chrono::high_resolution_clock::now();

    const int numHilos = std::thread::hardware_concurrency() > 0 ? 
                         std::thread::hardware_concurrency() : 4;
    
    std::vector<std::thread> hilos;
    int chunkSize = peliculas.size() / numHilos;
    std::mutex mutexInsert;

    for (int i = 0; i < numHilos; ++i) {
        int inicio = i * chunkSize;
        int fin = (i == numHilos - 1) ? peliculas.size() : inicio + chunkSize;

        hilos.emplace_back([this, &peliculas, inicio, fin, &mutexInsert]() {
            for (int j = inicio; j < fin; ++j) {
                const Pelicula& p = peliculas[j];
                string contenido = p.titulo + " " + p.sinopsis + " " +
                                  p.director + " " + p.genero + " " + p.cast;
                
                string limpio = normalizar(contenido);
                vector<string> palabras = tokenizar(limpio);

                for (const string& pal : palabras) {
                    if (!pal.empty()) {
                        std::lock_guard<std::mutex> lock(mutexInsert);
                        insertar(pal, p.id);
                    }
                }
            }
        });
    }

    for (auto& hilo : hilos) hilo.join();

    auto endPar = std::chrono::high_resolution_clock::now();
    auto tiempoParalelo = std::chrono::duration_cast<std::chrono::milliseconds>(endPar - startPar).count();

    cout << "[Trie] Construido en " << tiempoParalelo 
         << " ms usando " << numHilos << " hilos." << endl;
}