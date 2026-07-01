# Programación III: Plataforma de Streaming

**Integrantes:**
- Ariel Mathias Fernando Choque Marcelo
- Marco Antonio Benito Corimanya Juarez 
- Ernesto Yen Mendoza Aguilar
- Adrian Gabriel Rojas Tejada
- Gabriel Omar Saavedra Peralta

---

## ¿Por qué elegimos el Trie?

Elegimos el Trie porque permite buscar palabras por prefijo de forma muy eficiente. Por ejemplo, si el usuario escribe "bat", el programa encuentra todas las películas que contienen palabras que empiezan con "bat" sin tener que revisar todo el dataset. Además es sencillo de implementar en C++ sin necesidad de librerías externas.

---

## Estructura de un nodo

Cada nodo del Trie tiene:
- 36 cajitas (una por cada letra a-z y cada número 0-9)
- Una marca que indica si en ese nodo termina una palabra
- Una lista que registra en qué películas apareció esa palabra y cuántas veces

---

## Diagrama del árbol

Ejemplo con las palabras "bat" y "bad":


  ```
raiz
└── b
    └── a
        ├── t (fin de palabra)
        └── d (fin de palabra)
```
*La marca de fin de palabra indica que ahí termina un token válido y se guarda el ID de la película correspondiente.*

---

## Algoritmo de inserción

1. *Proceso de Inserción*

Cada palabra que proviene del preprocesamiento se ingresa al Trie carácter por carácter.
Primero, se obtiene el índice del carácter actual, donde las letras a-z ocupan los índices 0-25 y los números 0-9 ocupan los índices 26-35.
Si el nodo correspondiente a ese índice no existe, se crea uno nuevo.
Al llegar al último carácter de la palabra, se marca ese nodo como fin de palabra y se incrementa el contador de la película correspondiente.

2. *Construcción del Trie*

Para construir el Trie completo, se itera sobre todas las películas del dataset.
Por cada película, se concatena su título, sinopsis, director, género y cast en un solo texto.
Ese texto pasa por la función *normalizar()* y *tokenizar()* para obtener las palabras limpias.
Finalmente, cada palabra se inserta en el Trie asociada al índice de su película.

3. *Pseudocódigo*

Funcion insertar(palabra, idPelicula): Algoritmo que ingresa una palabra al Trie y la asocia a una película.
```
ALGORITMO insertar(palabra, idPelicula)
nodo actual = raiz
PARA CADA caracter EN palabra:
    indice = obtenerIndice(caracter)
    SI indice ES invalido (-1) ENTONCES:
        CONTINUAR al siguiente caracter
    SI nodo actual.hijos[indice] ES nulo ENTONCES:
        crear nuevo NodoTrie en nodo actual.hijos[indice]
    nodo actual = nodo actual.hijos[indice]

nodo actual.esFin = verdadero
nodo actual.contadorPeliculas[idPelicula]++
FIN del ALGORITMO
```


Funcion construirTrie(peliculas): Función que itera todas las películas y las ingresa al Trie.
```
ALGORITMO construirTrie(peliculas)
PARA CADA pelicula EN peliculas CON indice i:
contenido = pelicula.titulo + pelicula.sinopsis +
pelicula.director + pelicula.genero + pelicula.cast
contenido limpio = normalizar(contenido)
palabras = tokenizar(contenido limpio)
    PARA CADA palabra EN palabras:
        insertar(palabra, i)

FIN del ALGORITMO
```

---

## Algoritmo de búsqueda

1. *Proceso de Búsqueda*

La frase o palabra ingresada por el usuario se procesa por la función *normalizar()* para eliminar tildes, caracteres especiales y convertir todo a minúsculas.
Se recorre el árbol nodo por nodo siguiendo los caracteres de la consulta. Si se interrumpe el camino significaría que no existen coincidencias.
Una vez alcanzado el último nodo de la consulta, se realiza un recorrido profundo sobre todo el subárbol. Esto permite encontrar películas donde el término buscado es solo el inicio de la palabra.

Ejemplo:

  ```
"bar" -> "barco","barista"
```

2. *Pseudocódigo*

Funcion buscar(consulta): Algoritmo que localiza el nodo del prefijo y realiza la recolección de datos.
  ```
ALGORITMO buscar(consulta)
consulta limpia = normalizar(consulta)
nodo actual = raiz
PARA CADA caracter EN consulta limpia:
    indice = obtenerIndice(caracter)
    SI nodo actual.hijos[indice] ES nulo ENTONCES:
        RETORNAR lista vacia
    nodo actual = nodo actual.hijos[indice]

resultados mapa = nuevo MapaVacio()
recolectarResultados(nodo actual, resultados mapa)

RETORNAR resultados mapa     
```


---

## Patrones de Diseño

### Singleton (Trie)
Se garantiza que solo existe una instancia del Trie en todo el programa. Construir el Trie es costoso porque recorre más de 34,000 películas, por lo que no tiene sentido permitir múltiples instancias.

### Strategy (Ranking)
Permite intercambiar el criterio de ordenamiento de resultados (por frecuencia o por similitud) sin modificar el código que llama a buscar().

### Observer (Likes)
Al agregar un Like, se notifica automáticamente a las partes del programa interesadas en ese evento, como la lista de similares, sin llamarlas manualmente desde el mismo lugar.

### Builder (Pelicula)
Separa la construcción del objeto Pelicula en pasos reutilizables, reemplazando la asignación manual campo por campo que existía en cargador_csv.cpp.

---

## Programación Paralela

construirTrie() fue paralelizado usando std::thread, dividiendo el dataset en bloques para que varios hilos inserten simultáneamente.

### Tabla comparativa de tiempos

| Modo | Tiempo de construcción |
|------|----------------------|
| Secuencial | (pendiente datos de P2) |
| Paralelo | (pendiente datos de P2) |

---

## Programación Genérica

(pendiente implementación de P1)

---

## Referencias

- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley.
- Gamma, E., Helm, R., Johnson, R., & Vlissides, J. (1994). *Design Patterns: Elements of Reusable Object-Oriented Software*. Addison-Wesley.
