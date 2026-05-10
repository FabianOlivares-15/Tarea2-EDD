En este *repl* puedes encontrar varios ejemplos que te pueden ayudar con las tareas de estructura.

## Código de (tarea2)
Para ejecutar el ejemplo tarea2 primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
````

Y luego ejecutar:
````
./tarea2
````

## TDAs
En la carpeta `tdas` se encuentran implementados distintos TDAs que puedes utilizar (lista, pila, cola, cola con prioridad y mapas). 

Las implementaciones no son las más eficientes (todas usan como estructura de datos una **lista enlazada**), por lo que puedes reemplazarlas por las que has realizado en los labs.

## TODAS LAS FUNCIONES SIRVEN CORRECTAMENTE
1. Cargar peliculas: Lee el archivo y carga toda la informacion en los mapas correspondientes, para luego hacer busquedas ultrarapidas, muestra lo que cargamos.
2. Buscar por ID: Recibimos un id, y mostramos con una busqueda rapida la informacion de la pelicula correspondiente a ese id.
3. Buscar por genero: Recibimos un genero, y mostramos todas las peliculas que comparten ese genero e informacion de estas.
4. Buscar por director: Recibimos el nombre de algun director(a), y mostramos todas las obras de este, con la informacion correspondiente.
5. Buscar por decada: Recibimos un año, calculamos la decada de este, y mostramos las peliculas lanzadas durante esa decada con su informacion.
6. Busqueda avanzada: Es un filtro de busqueda doble, donde recibimos un genero y un año, buscamos en el mapa de genero, y luego vamos preguntando si corresponde a la decada antes calculada con el año y se imprimen aquellas que cumplan con el genero y decada indicados.
7. Gestionar mi watchlist: Abre un submenu que crea una lista de favoritos, que permite agregar, eliminar y mostrar lo de esta lista.
   .Agregar ID: Valida que el ID que vamos a agregar a nuestra lista exista, si es asi guardamos la informacion de esta pelicula.
   .Eliminar pelicula: Buscamos el ID indicado dentro de nuestra lista y lo eliminamos.
   .Mostrar watchlist: Mostramos el contenido de nuestra lista.

## EJEMPLO DE FUNCIONAMIENTO
Iniciamos el programa y usamos la opción 1 para cargar el catálogo. El sistema lee el archivo y guarda los datos en los mapas.
Presionamos el 3, escribimos el género Comedy y el sistema imprime todas las comedias con sus años, ratings y directores.
Presionamos el 4, escribimos el director ej(Fabian Olivares) y el sistema imprime todas las obras de este director con sus años, ratings y generos.
Presionamos el 5, escribimos el año de lanzamiento y el sistema imprime todas las peliculas de la decada con su año, ratings y directores.
Queremos algo más específico, presionamos el 6 (Búsqueda avanzada). Escribimos el género Comedy y luego el año 1985. El programa nos muestra  las películas de comedia de la década de los 80s.
Vemos una película que nos gusta y copiamos su ID. Presionamos el 7 para entrar a la Watchlist y luego el 1 para agregar. Pegamos el ID y el sistema valida que existe, guardándola en nuestros favoritos.
Volvemos al submenú de la Watchlist (opción 7) y presionamos el 3. El sistema nos muestra nuestra lista de favoritos con los títulos que hemos ido agregando.
Si ya vimos una de las películas, entramos al menú 7, elegimos la opción 2 (Eliminar), ingresamos el ID de la película y esta se borra de nuestra lista personal.
