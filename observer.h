#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>
#include "trie.h"

using namespace std;

class Observer
{
public:
    virtual void update(int idPelicula) = 0;
    virtual ~Observer() = default;
};

class LikeSubject
{
private:
    vector<Observer*> observers;

public:
    void attach(Observer* observer);

    void detach(Observer* observer);

    void notify(int idPelicula);
};

class SimilaresObserver : public Observer
{
private:
    Trie& trie;
    const vector<Pelicula>& peliculas;
    vector<Resultado> recomendaciones;

public:
    SimilaresObserver(Trie& trie,
                      const vector<Pelicula>& peliculas);

    void update(int idPelicula) override;

    const vector<Resultado>& obtenerRecomendaciones() const;
};

#endif