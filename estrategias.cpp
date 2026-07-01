//
// Created by gteja on 1/07/2026.
//

#include "estrategias.h"

vector<Resultado> RankingPorFrecuencia::rankear(const unordered_map<int, int>& resultadosBrutos, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes) {
    auto comp = [](const Resultado& a, const Resultado& b) {
        return a.relevancia < b.relevancia;
    };
    priority_queue<Resultado, vector<Resultado>, decltype(comp)> pq(comp);

    for (auto const& [id, frec] : resultadosBrutos) {
        pq.push({id, frec});
    }

    vector<Resultado> listaOrdenada;
    while (!pq.empty()) {
        listaOrdenada.push_back(pq.top());
        pq.pop();
    }
    return listaOrdenada;
}

// --- ESTRATEGIA 2: SIMILITUD ---
int RankingPorSimilitud::calcularPuntajeSimilitud(const Pelicula& candidata, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes) {
    int puntaje = 0;

    for (int idLike : listaLikes) {
        const Pelicula& peliGustada = baseDatos[idLike];

        if (candidata.director != "Unknown" && candidata.director == peliGustada.director) {
            puntaje += 50;
        }

        if (candidata.genero != "Unknown" && candidata.genero == peliGustada.genero) {
            puntaje += 30;
        }
    }
    return puntaje;
}

vector<Resultado> RankingPorSimilitud::rankear(const unordered_map<int, int>& resultadosBrutos, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes) {
    auto comp = [](const Resultado& a, const Resultado& b) {
        return a.relevancia < b.relevancia;
    };
    priority_queue<Resultado, vector<Resultado>, decltype(comp)> pq(comp);

    for (auto const& [id, frec] : resultadosBrutos) {
        int puntajeSimilitud = calcularPuntajeSimilitud(baseDatos[id], baseDatos, listaLikes);
        int puntajeFinal = puntajeSimilitud + frec;

        pq.push({id, puntajeFinal});
    }

    vector<Resultado> listaOrdenada;
    while (!pq.empty()) {
        listaOrdenada.push_back(pq.top());
        pq.pop();
    }
    return listaOrdenada;
}