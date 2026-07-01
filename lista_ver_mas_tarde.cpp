#include "lista_ver_mas_tarde.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

vector<int> listaVerMasTarde;
vector<int> listaLikes;

void cargarListas() {
    ifstream archivoVer("ver_mas_tarde.txt");
    int id;
    while (archivoVer >> id) {
        listaVerMasTarde.push_back(id);
    }
    archivoVer.close();

    ifstream archivoLikes("likes.txt");
    while (archivoLikes >> id) {
        listaLikes.push_back(id);
    }
    archivoLikes.close();
}

void guardarListas() {
    ofstream archivoVer("ver_mas_tarde.txt");
    for (int id : listaVerMasTarde) {
        archivoVer << id << "\n";
    }
    archivoVer.close();

    ofstream archivoLikes("likes.txt");
    for (int id : listaLikes) {
        archivoLikes << id << "\n";
    }
    archivoLikes.close();
}

void agregarVerMasTarde(int idPelicula) {
    listaVerMasTarde.push_back(idPelicula);
    //ya no guarda inmediatamente, se guarda al salir
}

void agregarLike(int idPelicula) {
    listaLikes.push_back(idPelicula);
    //ya no guarda inmediatamente, se guarda al salir
}

void mostrarVerMasTarde(const vector<Pelicula>& peliculas) {
    if (listaVerMasTarde.empty()) {
        cout << "Tu lista de Ver mas tarde esta vacia.\n";
        return;
    }
    cout << "=== Ver mas tarde ===\n";
    for (int id : listaVerMasTarde) {
        if (id >= 0 && id < (int)peliculas.size()) {
            cout << "- " << peliculas[id].titulo
                 << " (" << peliculas[id].releaseYear << ") "
                 << "[" << peliculas[id].genero << "]\n";
        }
    }
}