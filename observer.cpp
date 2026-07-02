#include "observer.h"
#include "estrategias.h"
#include "lista_ver_mas_tarde.h"

SimilaresObserver::SimilaresObserver(
        Trie& trie,
        const vector<Pelicula>& peliculas)
        : trie(trie), peliculas(peliculas)
{
}

void LikeSubject::attach(Observer* observer)
{
    observers.push_back(observer);
}

void LikeSubject::detach(Observer* observer)
{
    observers.erase(
            remove(observers.begin(),
                   observers.end(),
                   observer),
            observers.end());
}

void LikeSubject::notify(int idPelicula)
{
    for(Observer* observer : observers)
    {
        observer->update(idPelicula);
    }
}

void SimilaresObserver::update(int idPelicula)
{
    trie.setEstrategia(new RankingPorSimilitud());

    recomendaciones = trie.buscar(
            peliculas[idPelicula].titulo,
            peliculas,
            listaLikes
    );

    trie.setEstrategia(new RankingPorFrecuencia());
}

const vector<Resultado>&
SimilaresObserver::obtenerRecomendaciones() const
{
    return recomendaciones;
}