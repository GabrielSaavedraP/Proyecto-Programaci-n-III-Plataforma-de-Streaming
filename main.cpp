#include <iostream>
#include "cargador_csv.h"
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

#include "preprocesador.h"
#include "trie.h"
#include "lista_ver_mas_tarde.h"

using namespace std;

int main() {

    /* 
    Prueba trie: 
    Trie& trie = Trie::getInstance();
    trie.construirTrie(peliculas);
 */

    vector<Pelicula> pelis = cargarCSV("../wiki_movie_plots_deduped.csv");
    for (int i = 0; i < 4; i++) {
        string texto = pelis[i].titulo + " " + pelis[i].sinopsis;
        string limpio = normalizar(texto);
        vector<string> palabras = tokenizar(limpio);
        cout << "Pelicula: " << pelis[i].titulo << endl;
        for (int j = 0; j < 5 && j < palabras.size(); j++) {
            cout << palabras[j] << " ";
        }
        cout << "\n-----------------\n";
    }

    Trie& trie = Trie::getInstance();
    cout << "Construyendo estructura de busqueda.... " << endl;
    trie.construirTrie(pelis);
    cout << "Trie construido con exito" << endl;

    /*
    string consulta;
    cout << "\nIngrese su la busqueda que desea: ";
    cin >> consulta;

    vector <Resultado> resultados = miTrie.buscar(consulta);
    if (resultados.empty()) {
        cout << "No se encontraron coincidencias para: " << consulta << endl;
    } else {
        cout << "Se encontraron " << resultados.size() << " coinciencias." << endl;
        int paginaActual = 1;
        vector<Resultado> pagina = miTrie.obtenerPagina(resultados, paginaActual);
        cout << "\n--- Resulados Pagina " << paginaActual << " (Top 5) ---" << endl;
        for (const auto& res: pagina) {
            cout << "ID: " << res.idPelicula
                 << " | Titulo " << pelis[res.idPelicula].titulo
                 << " | Relevancia: " << res.relevancia << endl;
        }
    }
    */

    cargarListas();

    int opcion;

    do {

        cout << "\n====================================\n";
        cout << "        PLATAFORMA STREAMING\n";
        cout << "====================================\n";
        cout << "1. Buscar pelicula\n";
        cout << "2. Ver lista Ver mas tarde\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        // [TESTEO P4: No finalizado aun]
        if (opcion == 1) {
            cin.ignore();
            string consulta;
            cout << "\nIngrese palabra, frase o sub-palabra a buscar: ";
            getline(cin, consulta);
            vector<Resultado> resultados = miTrie.buscar(consulta);
            if (resultados.empty()) {
                cout << "\nNo se encontraron coincidencias para: "
                     << consulta << endl;
            } else {
                int paginaActual = 1;
                char accion;
                do {
                    vector<Resultado> pagina =
                            miTrie.obtenerPagina(resultados,
                                                 paginaActual,
                                                 5);
                    cout << "\n====================================\n";
                    cout << " RESULTADOS - PAGINA "
                         << paginaActual << endl;
                    cout << "====================================\n";
                    cout << "Total encontrados: "
                         << resultados.size() << "\n\n";
                    for (int i = 0; i < pagina.size(); i++) {
                        int id = pagina[i].idPelicula;
                        cout << i + 1 << ". "
                             << pelis[id].titulo
                             << " ("
                             << pelis[id].releaseYear
                             << ") - "
                             << pelis[id].genero
                             << " | Relevancia: "
                             << pagina[i].relevancia
                             << endl;
                    }
                    cout << "\nOpciones:\n";
                    cout << "[1-5] Ver detalle\n";
                    cout << "[N] Siguiente pagina\n";
                    cout << "[P] Pagina anterior\n";
                    cout << "[B] Volver\n";
                    cout << "Seleccione: ";
                    cin >> accion;
                    accion = tolower(accion);
                    // [TESTEO P4: No finalizado aun]
                    if (accion >= '1' && accion <= '5') {
                        int indice = accion - '1';
                        if (indice < pagina.size()) {
                            int id = pagina[indice].idPelicula;
                            char detalleOpcion;
                            do {
                                cout << "\n====================================\n";
                                cout << "          DETALLE PELICULA\n";
                                cout << "====================================\n";
                                cout << "Titulo: "
                                     << pelis[id].titulo << endl;
                                cout << "Anio: "
                                     << pelis[id].releaseYear << endl;
                                cout << "Genero: "
                                     << pelis[id].genero << endl;
                                cout << "Director: "
                                     << pelis[id].director << endl;
                                cout << "Cast: "
                                     << pelis[id].cast << endl;
                                cout << "\nSinopsis:\n"
                                     << pelis[id].sinopsis << endl;
                                cout << "\nOpciones:\n";
                                cout << "[L] Like\n";
                                cout << "[W] Ver mas tarde\n";
                                cout << "[B] Volver\n";
                                cout << "Seleccione: ";
                                cin >> detalleOpcion;
                                detalleOpcion = tolower(detalleOpcion);
                                // [TESTEO P4: No finalizado aun]
                                if (detalleOpcion == 'l') {
                                    agregarLike(id);
                                    cout << "\nPelicula agregada a Likes.\n";
                                }
                                else if (detalleOpcion == 'w') {
                                    agregarVerMasTarde(id);
                                    cout << "\nPelicula agregada a Ver mas tarde.\n";
                                }
                            } while (detalleOpcion != 'b');
                        }
                    }
                    // [TESTEO P4: No finalizado aun]
                    else if (accion == 'n') {
                        vector<Resultado> siguiente =
                                miTrie.obtenerPagina(resultados,
                                                     paginaActual + 1,
                                                     5);
                        if (!siguiente.empty()) {
                            paginaActual++;
                        } else {
                            cout << "\nNo hay mas resultados.\n";
                        }
                    }
                    else if (accion == 'p') {
                        if (paginaActual > 1) {
                            paginaActual--;
                        } else {
                            cout << "\nYa estas en la primera pagina.\n";
                        }
                    }
                } while (accion != 'b');
            }
        }

        else if (opcion == 2) {
            cout << "\n";
            mostrarVerMasTarde(pelis);
        }
        else if (opcion == 3) {
            guardarListas();
            cout << "\nSaliendo del programa...\n";
        }
        else {
            cout << "\nOpcion invalida.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }

    } while (opcion != 3);
    return 0;
}