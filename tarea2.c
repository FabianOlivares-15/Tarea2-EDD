#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  char id[100];
  char title[100];
  List *director;
  List *genres;
  int year;
  float rating;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por genero");
  puts("4) Buscar por director");
  puts("5) Buscar por decada");
  puts("6) Busqueda avanzada");
  puts("7) Gestionar mi watchlist");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirector, Map *pelis_bydecada) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    peli->director = split_string(campos[14], ",");
    peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
    peli->year = atoi(campos[10]);      // Asigna año, convirtiendo de cadena a entero
    peli->rating = atof(campos[8]);    
    
    
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
  
    // Obtiene el primer género de la lista de géneros de la película
    char *genre = list_first(peli->genres);
    // Itera sobre cada género de la película
    while (genre != NULL) {
        // Busca el género en el mapa pelis_bygenres
        MapPair *genre_pair = map_search(pelis_bygenres, genre);

        // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
        if (genre_pair == NULL) {
            List *new_list = list_create();
            list_pushBack(new_list, peli);
            map_insert(pelis_bygenres, genre, new_list);
        } else {
            // Si el género ya existe en el mapa, obtén la lista y agrega la película
            List *genre_list = (List *)genre_pair->value;
            list_pushBack(genre_list, peli);
        }

        // Avanza al siguiente género en la lista
        genre = list_next(peli->genres);
    }
    char *director =list_first(peli->director);
    while(director != NULL){
      MapPair *directorPair = map_search(pelis_bydirector, director);
      if(directorPair == NULL){
        List *listaNuevaDirec = list_create();
        list_pushBack(listaNuevaDirec, peli);
        map_insert(pelis_bydirector, director, listaNuevaDirec);
      }
      else{
        List *directorList = (List*)directorPair->value;
        list_pushBack(directorList, peli);
      }
      director = list_next(peli->director);
    }
    int decada = (peli->year / 10) * 10;
    MapPair *decadaPair = map_search(pelis_bydecada, &decada);
    if(decadaPair == NULL){
      List *nuevaListaDeca = list_create();
      list_pushBack(nuevaListaDeca, peli);
      int *decadaKey = (int*)malloc(sizeof(int));
      *decadaKey = decada;
      map_insert(pelis_bydecada, decadaKey, nuevaListaDeca);
    }
    else{
      List *decadaList = (List*)decadaPair->value;
      list_pushBack(decadaList, peli);
    }
    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas


  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title,
           peli->year);

    printf("Géneros: ");
    for(char *genre = list_first(peli->genres); genre != NULL; genre = list_next(peli->genres))
      printf("%s, ", genre);
    printf(" ");
    printf("Director(es): ");
    for(char *director = list_first(peli->director); director != NULL; director = list_next(peli->director))
      printf("%s, ", director);
    printf("\n");
    
    
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}
void buscar_por_genre(Map *pelis_bygenres){
  char genre[25];
  printf("Ingrese el genero de la pelicula: ");
  scanf(" %[^\n]", genre);
  MapPair *pair = map_search(pelis_bygenres, genre);
  if(pair != NULL){
    List *peliculasDeGenre = (List*)pair->value;
    Film *peli = (Film*)list_first(peliculasDeGenre);
    printf("\n Peliculas del genero %s \n", genre);
    while(peli != NULL){
      printf(" Titulo: %s | Año: (%d) | Rating: %1.f | Director(es): ", peli->title, peli->year, peli->rating);
      char *dir = (char*)list_first(peli->director);
      while(dir != NULL){
        printf("%s", dir);
        dir = (char*)list_next(peli->director);
        if(dir != NULL){
          printf(", ");
        }
      }
      printf("\n");
      peli = (Film*)list_next(peliculasDeGenre);
    }
  }
  else{
    printf("No se encontraron peliculas del genero %s\n", genre);
  }
}

void buscar_por_director(Map *pelis_bydirector){
  char director[25];
  printf("Ingrese el director de la pelicula: ");
  scanf(" %[^\n]", director);
  MapPair *pair = map_search(pelis_bydirector, director);
  if(pair != NULL){
    List *peliculasDeDirector = (List*)pair->value;
    Film *peli = (Film*)list_first(peliculasDeDirector);
    printf("\nPeliculas del director %s \n", director);
    while(peli != NULL){
      printf("Titulo: %s | Año: (%d) | Rating: %1.f | Genero(s): ", peli->title, peli->year, peli->rating);
      char *genre = (char*)list_first(peli->genres);
      while(genre != NULL){
        printf("%s", genre);
        genre = (char*)list_next(peli->genres);
        if(genre != NULL){
          printf(", ");
        }
      }
      printf("\n");
      peli = (Film*)list_next(peliculasDeDirector);
    }
  }
  else{
    printf("No se encontraron peliculas del director %s\n", director);
  }
}

void buscar_por_decada(Map *pelis_bydecada){
  int anioPeliculas;
  printf("Ingrese el año para buscar su decada: ");
  scanf("%d", &anioPeliculas);

  int buscarDecada = (anioPeliculas / 10) * 10;
  MapPair *pair = map_search(pelis_bydecada, &buscarDecada);
  if(pair != NULL){
    List *peliculasPorDecada = (List*)pair->value;
    Film *peli = (Film*)list_first(peliculasPorDecada);
    printf("Peliculas de la decada de los %d\n", buscarDecada);
    while(peli != NULL){
      printf("Titulo: %s | Año: (%d) | Rating: %1.f | Director(es): ", peli->title, peli->year, peli->rating);
      char *director = (char*)list_first(peli->director);
      while(director != NULL){
        printf("%s", director);
        director = (char*)list_next(peli->director);
        if(director != NULL){
          printf(", ");
        }
      }
      printf("\n");
      peli = (Film*)list_next(peliculasPorDecada);
    }
  }
  else{
    printf("No se encontraron peliculas de esta decada %d\n", buscarDecada);
  }
}

void busqueda_avanzada(Map *pelis_bygenres){
  char genre[25];
  int anioBuscar;
  printf("Ingrese el genero: ");
  scanf(" %[^\n]", genre);
  
  printf("Ingrese el año para buscar la decada: ");
  scanf("%d", &anioBuscar);
  int decadaBuscada = (anioBuscar / 10) * 10;
  MapPair *pair = map_search(pelis_bygenres, genre);
  if(pair != NULL){
    List *listaPeliculas = (List*)pair->value;
    Film *peli = (Film*)list_first(listaPeliculas);
    bool encontradas = false;
    printf("Peliculas del genero %s en la decada de los %d\n", genre, decadaBuscada);
    while(peli != NULL){
      int decadaPeli = (peli->year / 10) * 10;
      if(decadaPeli == decadaBuscada){
        printf("Titulo: %s | Año: (%d) | Rating: %1.f\n", peli->title, peli->year, peli->rating);
        encontradas = true;
      }
      peli = (Film*)list_next(listaPeliculas);
    }
    if(!encontradas){
      printf("No hay peliculas del genero %s en la decada de los %d\n", genre, decadaBuscada);
    }
  }
  else{
    printf("No existe el genero %s\n", genre);
  }
}

void agregarIdWatchlist(Map *pelis_byid, List *miWatchList){
  char id[50];
  printf("Ingrese el id de la pelicula: ");
  scanf(" %s", id);
  MapPair *pair = map_search(pelis_byid, id);
  if(pair == NULL){
    printf("Este id %s no existe en el catalogo\n", id);
  }
  else{
    Film *peli = (Film*)pair->value;
    list_pushBack(miWatchList, peli);
    printf("La pelicula llamada %s fue agregada\n", peli->title);
  }
}

void eliminarDeWatchlist(List *miWatchlist){
  char id[50];
  printf("Ingrese el id de la pelicula a eliminar: ");
  scanf(" %s", id);
  Film *peli = (Film*)list_first(miWatchlist);
  bool encontrado = false;
  while(peli != NULL){
    if(strcmp(peli->id, id) ==0){
      list_popCurrent(miWatchlist);
      printf("Se elimino la pelicula %s\n", peli->title);
      encontrado = true;
      break;
    }
    peli = (Film*)list_next(miWatchlist);
  }
  if(!encontrado){
    printf("Este id %s no esta en tu watchlist\n", id);
  }
}

void mostrarWatchlist(List *miWatchlist){
  printf("-----Mi Watchlist-----\n");
  Film *peli = (Film*)list_first(miWatchlist);
  if(peli == NULL){
    printf("No tienes nada en tu watchlist\n");
  }
  else{
    while(peli != NULL){
      printf("ID: %s | Titulo: %s | Año: (%d) | Rating: %1.f\n", peli->id, peli->title, peli->year, peli->rating);
      peli = (Film*)list_next(miWatchlist);
    }
  }
}

void gestionar_watchlist(Map *pelis_byid, List *watchList){
  char opcion;
  do{
    limpiarPantalla();
    puts("========================================");
    puts("     Gestionar Mi Watchlist");
    puts("========================================");

    puts("1) Agregar pelicula por ID");
    puts("2) Eliminar pelicula");
    puts("3) Mostrar mi watchlist");
    puts("4) Salir al menu");
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    switch (opcion){
      case '1':
        agregarIdWatchlist(pelis_byid, watchList);
        break;
      case '2':
        eliminarDeWatchlist(watchList);
        break;
      case '3':
        mostrarWatchlist(watchList);
        break;
    }
    presioneTeclaParaContinuar();
    
  }while(opcion != '4');
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirector = map_create(is_equal_str);
  Map *pelis_bydecada = map_create(is_equal_int);
  List *miWatchlist = list_create();

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirector, pelis_bydecada);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      buscar_por_genre(pelis_bygenres);
      break;
    case '4':
      buscar_por_director(pelis_bydirector);
      break;
    case '5':
      buscar_por_decada(pelis_bydecada);
      break;
    case '6':
      busqueda_avanzada(pelis_bygenres);
      break;
    case '7':
      gestionar_watchlist(pelis_byid, miWatchlist);
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
