#include "preprocesador.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "pelicula.h"
#include "Resultado.h"


using namespace std;

unordered_set<string> stopwords = {
    "i","me","my","myself","we","our","ours","ourselves",
    "you","your","yours","yourself","yourselves",
    "he","him","his","himself","she","her","hers","herself",
    "it","its","itself","they","them","their","theirs","themselves",
    "what","which","who","whom","this","that","these","those",
    "am","is","are","was","were","be","been","being",
    "have","has","had","having",
    "do","does","did","doing",
    "a","an","the",
    "and","but","if","or","because","as","until","while",
    "of","at","by","for","with","about","against","between",
    "into","through","during","before","after","above","below",
    "to","from","up","down","in","out","on","off","over","under",
    "again","further","then","once",
    "here","there","when","where","why","how",
    "all","any","both","each","few","more","most","other",
    "some","such","no","nor","not","only","own","same",
    "so","than","too","very","can","will","just","should"
};

string normalizar(const string& texto) {
    string r = "";

    for (int i = 0; i < texto.size(); i++) {
        char c = texto[i];

        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }

        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ') {
            r += c;
        }
    }

    return r;
}




unordered_map<string, vector<int>> crearIndice(
    const vector<Pelicula>& peliculas,
    const string& campo)
{
    unordered_map<string, vector<int>> indice;

    for(int i = 0; i < peliculas.size(); i++)
    {
        string texto;

        if(campo == "director")
            texto = peliculas[i].director;

        else if(campo == "genero")
            texto = peliculas[i].genero;

        else if(campo == "cast")
            texto = peliculas[i].cast;

        else
            continue;

        texto = normalizar(texto);

        vector<string> palabras = tokenizar(texto);

        for(const string& palabra : palabras)
        {
            indice[palabra].push_back(i);
        }
    }

    return indice;
}

vector<Resultado> buscarPorTag(
    const unordered_map<string, vector<int>>& indice,
    const string& consulta)
{
    string limpio = normalizar(consulta);
    vector<string> palabras = tokenizar(limpio);

    unordered_map<int, int> consolidado;

    // Cuenta cuántas veces aparece cada película
    for (const string& palabra : palabras)
    {
        auto it = indice.find(palabra);

        if (it != indice.end())
        {
            for (int id : it->second)
            {
                consolidado[id]++;
            }
        }
    }

    priority_queue<Resultado> pq;

    for (const auto& [id, frecuencia] : consolidado)
    {
        pq.push({id, frecuencia});
    }

    vector<Resultado> resultados;

    while (!pq.empty())
    {
        resultados.push_back(pq.top());
        pq.pop();
    }

    return resultados;
}