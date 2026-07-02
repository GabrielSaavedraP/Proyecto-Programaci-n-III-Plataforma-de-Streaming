#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <stdexcept>

#include "cargador_csv.h"
#include "preprocesador.h"
#include "trie.h"
#include "lista_ver_mas_tarde.h"
#include "observer.h"

using namespace std;

int main()
{
    try
    {
        vector<Pelicula> pelis =
                cargarCSV("../wiki_movie_plots_deduped.csv");

        if(pelis.empty())
            throw runtime_error("No se pudo cargar el CSV.");

        Trie& trie = Trie::getInstance();

        cout << "Construyendo estructura de busqueda...\n";
        trie.construirTrie(pelis);
        cout << "Trie construido correctamente.\n";

        auto indiceDirector = crearIndice(pelis,"director");
        auto indiceGenero   = crearIndice(pelis,"genero");
        auto indiceCast     = crearIndice(pelis,"cast");

        LikeSubject likeSubject;
        SimilaresObserver observer(trie, pelis);
        likeSubject.attach(&observer);

        cargarListas();

        cout << "\n=====================================\n";
        cout << "VER MAS TARDE\n";
        cout << "=====================================\n";

        mostrarVerMasTarde(pelis);

        cout << "\n=====================================\n";
        cout << "RECOMENDACIONES PARA TI\n";
        cout << "=====================================\n";

        if(!listaLikes.empty())
        {
            likeSubject.notify(listaLikes.back());

            const vector<Resultado>& similares =
                    observer.obtenerRecomendaciones();

            int mostrados = 0;

            for(const Resultado& r : similares)
            {
                if(r.idPelicula == listaLikes.back())
                    continue;

                cout
                    << mostrados + 1
                    << ". "
                    << pelis[r.idPelicula].titulo
                    << " ("
                    << pelis[r.idPelicula].releaseYear
                    << ")\n";

                mostrados++;

                if(mostrados == 5)
                    break;
            }

            if(mostrados == 0)
            {
                cout << "No hay recomendaciones disponibles.\n";
            }
        }
        else
        {
            cout << "Aun no tienes Likes registrados.\n";
        }


        int opcion;

        do
        {
            cout << "\n";
            cout << "=========================================\n";
            cout << "      PLATAFORMA DE STREAMING\n";
            cout << "=========================================\n";
            cout << "1. Buscar por palabra o frase\n";
            cout << "2. Buscar por Director\n";
            cout << "3. Buscar por Genero\n";
            cout << "4. Buscar por Cast\n";
            cout << "5. Ver lista Ver mas tarde\n";
            cout << "6. Salir\n";
            cout << "Seleccione una opcion: ";

            if(!(cin>>opcion))
            {
                cin.clear();
                cin.ignore(10000,'\n');
                throw runtime_error("Debe ingresar un numero.");
            }

            cin.ignore();

            if(opcion==1)
            {
                string consulta;

                cout<<"\nIngrese palabra o frase: ";

                getline(cin,consulta);

                if(consulta.empty())
                    throw runtime_error("La consulta no puede estar vacia.");

                vector<Resultado> resultados =
                        trie.buscar(
                                consulta,
                                pelis,
                                listaLikes
                        );

                if(resultados.empty())
                {
                    cout<<"\nNo se encontraron resultados.\n";
                    continue;
                }

                int paginaActual=1;

                char accion;

                do
                {
                    vector<Resultado> pagina =
                            trie.obtenerPagina(
                                    resultados,
                                    paginaActual,
                                    5
                            );

                    cout<<"\n=====================================\n";
                    cout<<"RESULTADOS - PAGINA "<<paginaActual<<endl;
                    cout<<"=====================================\n";

                    cout<<"Total encontrados: "
                        <<resultados.size()
                        <<"\n\n";

                    for(int i=0;i<(int)pagina.size();i++)
                    {
                        int id=pagina[i].idPelicula;

                        cout
                                <<i+1
                                <<". "
                                <<pelis[id].titulo
                                <<" ("
                                <<pelis[id].releaseYear
                                <<")\n";

                        cout<<"Genero: "
                            <<pelis[id].genero
                            <<endl;

                        cout<<"Relevancia: "
                            <<pagina[i].relevancia
                            <<"\n\n";
                    }

                    cout<<"Opciones\n";
                    cout<<"[1-5] Ver detalle\n";
                    cout<<"[N] Pagina siguiente\n";
                    cout<<"[P] Pagina anterior\n";
                    cout<<"[B] Volver\n";
                    cout<<"Seleccione: ";

                    cin>>accion;

                    accion=tolower(accion);
                                        if(accion>='1' && accion<='5')
                    {
                        int indice=accion-'1';

                        if(indice<(int)pagina.size())
                        {
                            int id=pagina[indice].idPelicula;

                            char detalle;

                            do
                            {
                                cout<<"\n=====================================\n";
                                cout<<"DETALLE DE LA PELICULA\n";
                                cout<<"=====================================\n";

                                cout<<"Titulo    : "<<pelis[id].titulo<<endl;
                                cout<<"Año       : "<<pelis[id].releaseYear<<endl;
                                cout<<"Genero    : "<<pelis[id].genero<<endl;
                                cout<<"Director  : "<<pelis[id].director<<endl;
                                cout<<"Cast      : "<<pelis[id].cast<<endl;

                                cout<<"\nSinopsis\n";
                                cout<<"-------------------------------------\n";
                                cout<<pelis[id].sinopsis<<endl;

                                cout<<"\nOpciones\n";
                                cout<<"L -> Dar Like\n";
                                cout<<"W -> Ver mas tarde\n";
                                cout<<"B -> Volver\n";
                                cout<<"Seleccione: ";

                                cin>>detalle;

                                detalle=tolower(detalle);

                                if(detalle=='l')
                                {
                                    agregarLike(id);

                                    likeSubject.notify(id);

                                    cout<<"\nLike agregado correctamente.\n";

                                    const vector<Resultado>& similares =
                                            observer.obtenerRecomendaciones();

                                    if(!similares.empty())
                                    {
                                        cout<<"\n=====================================\n";
                                        cout<<"RECOMENDACIONES PARA TI\n";
                                        cout<<"=====================================\n";

                                        int mostrados=0;

                                        for(const Resultado& r : similares)
                                        {
                                            if(r.idPelicula==id)
                                                continue;

                                            cout
                                                <<mostrados+1
                                                <<". "
                                                <<pelis[r.idPelicula].titulo
                                                <<" ("
                                                <<pelis[r.idPelicula].releaseYear
                                                <<")"
                                                <<endl;

                                            mostrados++;

                                            if(mostrados==5)
                                                break;
                                        }

                                        if(mostrados==0)
                                        {
                                            cout<<"No existen recomendaciones disponibles.\n";
                                        }
                                    }
                                    else
                                    {
                                        cout<<"\nAun no existen recomendaciones.\n";
                                    }
                                }

                                else if(detalle=='w')
                                {
                                    agregarVerMasTarde(id);

                                    cout<<"\nPelicula agregada a Ver mas tarde.\n";
                                }

                                else if(detalle!='b')
                                {
                                    cout<<"\nOpcion invalida.\n";
                                }

                            }
                            while(detalle!='b');
                        }
                        else
                        {
                            cout<<"\nIndice fuera de rango.\n";
                        }
                    }

                    else if(accion=='n')
                    {
                        vector<Resultado> siguiente =
                                trie.obtenerPagina(
                                        resultados,
                                        paginaActual+1,
                                        5);

                        if(!siguiente.empty())
                        {
                            paginaActual++;
                        }
                        else
                        {
                            cout<<"\nNo existen mas paginas.\n";
                        }
                    }

                    else if(accion=='p')
                    {
                        if(paginaActual>1)
                        {
                            paginaActual--;
                        }
                        else
                        {
                            cout<<"\nYa se encuentra en la primera pagina.\n";
                        }
                    }

                    else if(accion!='b')
                    {
                        cout<<"\nOpcion invalida.\n";
                    }

                }
                while(accion!='b');
            }

            else if(opcion==2)
            {
                string consulta;

                cout<<"\nIngrese Director: ";

                getline(cin,consulta);

                vector<Resultado> resultados =
                        buscarPorTag(
                                indiceDirector,
                                consulta
                        );

                if(resultados.empty())
                {
                    cout<<"\nNo se encontraron peliculas.\n";
                }
                else
                {
                    cout<<"\n=========== RESULTADOS ===========\n";

                    for(size_t i=0;i<resultados.size();i++)
                    {
                        cout
                            <<i+1
                            <<". "
                            <<pelis[resultados[i].idPelicula].titulo
                            <<" ("
                            <<pelis[resultados[i].idPelicula].releaseYear
                            <<")"
                            <<endl;
                    }
                }
            }


            else if(opcion==3)
            {
                string consulta;

                cout<<"\nIngrese Genero: ";

                getline(cin,consulta);

                vector<Resultado> resultados =
                        buscarPorTag(
                                indiceGenero,
                                consulta
                        );

                if(resultados.empty())
                {
                    cout<<"\nNo se encontraron peliculas.\n";
                }
                else
                {
                    cout<<"\n=========== RESULTADOS ===========\n";

                    for(size_t i=0;i<resultados.size();i++)
                    {
                        int id = resultados[i].idPelicula;

                        cout
                            <<i+1
                            <<". "
                            <<pelis[id].titulo
                            <<" ("
                            <<pelis[id].releaseYear
                            <<")"
                            <<" | "
                            <<pelis[id].director
                            <<endl;
                    }
                }
            }

            else if(opcion==4)
            {
                string consulta;

                cout<<"\nIngrese Actor/Actriz: ";

                getline(cin,consulta);

                vector<Resultado> resultados =
                        buscarPorTag(
                                indiceCast,
                                consulta
                        );

                if(resultados.empty())
                {
                    cout<<"\nNo se encontraron peliculas.\n";
                }
                else
                {
                    cout<<"\n=========== RESULTADOS ===========\n";

                    for(size_t i=0;i<resultados.size();i++)
                    {
                        int id=resultados[i].idPelicula;

                        cout
                            <<i+1
                            <<". "
                            <<pelis[id].titulo
                            <<" ("
                            <<pelis[id].releaseYear
                            <<")"
                            <<endl;
                    }
                }
            }


            else if(opcion==5)
            {
                cout<<endl;

                mostrarVerMasTarde(pelis);
            }


            else if(opcion==6)
            {
                guardarListas();

                cout<<"\nGuardando informacion...\n";
                cout<<"Hasta luego.\n";
            }

            else
            {
                cout<<"\nOpcion invalida.\n";
            }

        }
        while(opcion!=6);
    }

    catch(const exception& e)
    {
        cout<<"\n=====================================\n";
        cout<<"ERROR\n";
        cout<<"=====================================\n";
        cout<<e.what()<<endl;
    }

    return 0;
}
