#include "trie.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Singleton
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

NodoTrie::NodoTrie() {
    for (int i = 0; i < 36; i++) hijos[i] = nullptr;
    esFin = false;
}

NodoTrie::~NodoTrie() {
    for (int i = 0; i < 36; i++) {
        delete hijos[i];
    }
}

// Metodos internos (igual que el proyecto anteriro)
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

// Funcion auxiliar para insertar pelicula 
void insertarPelicula(Trie* trie, const Pelicula& p, std::mutex& mutexInsert) {
    string contenido = p.titulo + " " + p.sinopsis + " " +
                       p.director + " " + p.genero + " " + p.cast;

    string limpio = normalizar(contenido);
    vector<string> palabras = tokenizar(limpio);

    for (const string& pal : palabras) {
        if (!pal.empty()) {
            std::lock_guard<std::mutex> lock(mutexInsert);
            trie->insertar(pal, p.id);      
        }
    }
}

// Construccion pra comparacion 
void Trie::construirTrie(const vector<Pelicula>& peliculas) {
    if (peliculas.empty()) return;

    cout << "[Trie] Iniciando construcción del índice...\n";

    // Version secuencial
    auto startSeq = std::chrono::high_resolution_clock::now();
    Trie trieSecuencial;  // Trie temporal para medición
    for (const auto& p : peliculas) {
        insertarPelicula(&trieSecuencial, p, *(new std::mutex())); // mutex dummy
    }
    auto endSeq = std::chrono::high_resolution_clock::now();
    auto tiempoSecuencial = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count();

    // Version con progra paralela
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
                insertarPelicula(this, peliculas[j], mutexInsert);
            }
        });
    }

    for (auto& hilo : hilos) hilo.join();

    auto endPar = std::chrono::high_resolution_clock::now();
    auto tiempoParalelo = std::chrono::duration_cast<std::chrono::milliseconds>(endPar - startPar).count();

    // Mostrar resultados
    cout << "[Trie] Construcción finalizada!" << endl;
    cout << "   → Tiempo Secuencial : " << tiempoSecuencial << " ms" << endl;
    cout << "   → Tiempo Paralelo   : " << tiempoParalelo << " ms (" 
         << numHilos << " hilos)" << endl;
    cout << "   → Mejora            : " << (double)tiempoSecuencial / tiempoParalelo << "x" << endl;
}    