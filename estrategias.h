//
// Created by gteja on 1/07/2026.
//

#ifndef PROYECTO_PROGRA_3_ESTRATEGIAS_H
#define PROYECTO_PROGRA_3_ESTRATEGIAS_H
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include "pelicula.h"
#include "Resultado.h"
using namespace std;

class EstrategiaRanking {
public:
    virtual vector<Resultado> rankear(const unordered_map<int, int>& resultadosBrutos,
                                      const vector<Pelicula>& baseDatos,
                                      const vector<int>& listaLikes) = 0;
    virtual ~EstrategiaRanking() {}
};

// Estrategia 1: Ordenar solo por las veces que aparece la palabra
class RankingPorFrecuencia : public EstrategiaRanking {
public:
    vector<Resultado> rankear(const unordered_map<int, int>& resultadosBrutos,
                              const vector<Pelicula>& baseDatos,
                              const vector<int>& listaLikes) override;
};

// Estrategia 2: Ordenar considerando los Likes del usuario (Similitud)
class RankingPorSimilitud : public EstrategiaRanking {
private:
    int calcularPuntajeSimilitud(const Pelicula& candidata, const vector<Pelicula>& baseDatos, const vector<int>& listaLikes);
public:
    vector<Resultado> rankear(const unordered_map<int, int>& resultadosBrutos,
                              const vector<Pelicula>& baseDatos,
                              const vector<int>& listaLikes) override;
};

#endif //PROYECTO_PROGRA_3_ESTRATEGIAS_H