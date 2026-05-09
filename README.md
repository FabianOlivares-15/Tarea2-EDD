En este *repl* puedes encontrar varios ejemplos que te pueden ayudar con las tareas de estructura.

## Código de (tarea1)
Para ejecutar el ejemplo tarea1 primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
````

Y luego ejecutar:
````
./tarea1
````

## TDAs
En la carpeta `tdas` se encuentran implementados distintos TDAs que puedes utilizar (lista, pila, cola, cola con prioridad y mapas). 

Las implementaciones no son las más eficientes (todas usan como estructura de datos una **lista enlazada**), por lo que puedes reemplazarlas por las que has realizado en los labs.

## TODAS LAS FUNCIONES SIRVEN CORRECTAMENTE
1)Nueva Categoria: solo admite nombres de una palabra
2)Eliminar Categoria: elimina la categoria y todas sus tareas
3)Mostrar Categoria: muestra los nombres de las categorias existentes
4)Registrar Tarea: registra la descripcion, categoria y hora de la tarea
5)Atender Siguiente: sigue la regla FIFO, utiliza popFront para sacar el mas antiguo
6)Tablero General: mustra todas las tareas en cronologico
7)Filtrar Categoria: muestra las tareas de la categoria especificada

## EJEMPLO DE FUNCIONAMIENTO
Usamos el 1 y creamos categoria: Hogar
Usamos el 1 y creamos categoria: Trabajo
Registramos una tarea presionando 4, escribimos la categoria Hogar y agregamos la tarea barrer pieza
Registramos una tarea presionando 4, escribimos la categoria Tranajo y agregamos la tarea leer documentos
Registramos una tarea presionando 4, escribimos la categoria Hogar y agregamos la tarea cocinar
Presionamos el 3 y nos muestra las categorias existentes, Hogar y Trabajo
Presionamos el 6 y nos muestra las tareas ingresadas ordenadas por hora y su categoria
Presionamos el 7 y nos muestra las tareas de la categoria ingresada si ponemos Hogar nos muestra, barrer pieza y cocinar
Presionamos el 2, escogemos la categoria Trabajo y se elimina la tarea junto con la categoria
Presionamos el 5, el sistema muestra la descripcion, categoria y hora ingresada de la tarea y se elimina
