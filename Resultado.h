//
// Created by ERNESTO on 1/07/2026.
//
#ifndef RESULTADO_H
#define RESULTADO_H

struct Resultado
{
    int idPelicula;
    int relevancia;

    bool operator<(const Resultado& otro) const
    {
        return relevancia < otro.relevancia;
    }
};

#endif
