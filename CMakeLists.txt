cmake_minimum_required(VERSION 4.0)
project(Proyecto_Progra_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto_Progra_3
        main.cpp
        cargador_csv.h
        cargador_csv.cpp
        pelicula.h
        preprocesador.h
        preprocesador.cpp
        lista_ver_mas_tarde.h
        lista_ver_mas_tarde.cpp
        trie.h
        trie.cpp
        Resultado.h)