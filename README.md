📦 Proyecto juego de pacman

Este proyecto tiene como objetivo desarrollar el juego del pacman implementando algoritmos como: colisiones,  renderizado, A*, backtraking. Ademas de gráficas, puntuación, etc. 

📝 Descripción

El juego es conocido por su simplicidad y adictiva jugabilidad, así como por su icónico diseño de personajes y sonidos reconocibles. A lo largo de los años, "Pac-Man" ha sido adaptado a numerosas plataformas y ha inspirado una amplia gama de secuelas, spin-offs y productos relacionados. Es considerado uno de los videojuegos más influyentes y reconocibles de la historia.

🚀 Funcionalidades

Este código implementa el clásico juego de Pacman. Algunas de las funcionalidades principales del juego incluyen:

1. **Movimiento del personaje:** El código maneja el movimiento del personaje principal, Pacman, dentro del laberinto. Este movimiento es controlado por las teclas de dirección y se gestiona dentro de la función `dibujar_personaje()`.

2. **Dibujar el mapa:** Se dibuja el mapa del juego, que incluye los muros y la comida que Pacman debe recolectar. Los muros están representados por la letra 'X' y la comida por la letra 'o'. La función `dibujar_mapa()` se encarga de esta funcionalidad.

3. **Detección de colisiones:** El código detecta colisiones entre Pacman y los fantasmas o los muros. Si Pacman colisiona con un muro, su movimiento se detiene. Si colisiona con un fantasma, pierde una vida. Esto se gestiona dentro de la función `choque_pacman()`.

4. **Control de vidas:** El juego mantiene un contador de vidas para Pacman. Cuando Pacman colisiona con un fantasma, pierde una vida. Si se quedan sin vidas, el juego termina. Esto se controla dentro de las funciones `verificar_fin_de_juego()`, `perder_vida()`, `mostrar_mensaje_de_fin_de_juego()` y `dibujar_vidas()`.

5. **Interfaz de usuario:** El código solicita al jugador que ingrese su nombre al principio del juego y muestra las vidas restantes de Pacman en la pantalla.

Estas funcionalidades son esenciales para el funcionamiento del juego de Pacman y están implementadas en el código proporcionado.

💻 Tecnologías Utilizadas

C++ 

IDE codeblocks (Version: 17.12)

Allegro (libreria para la interface grafica)

GitHub (control de versiones y trabajo en equipo)

📁 Estructura del Proyecto

El proyecto sigue una estructura modular, dividida en los siguientes componentes:

El código proporcionado es un programa escrito en C++ utilizando la biblioteca Allegro para la creación de un juego similar al clásico Pac-Man. La estructura del código se puede dividir en varias secciones:

1. **Inclusión de bibliotecas y definiciones de constantes**: Al inicio del archivo se incluyen las bibliotecas necesarias para el funcionamiento del programa, así como definiciones de constantes y variables globales.

2. **Declaración de variables globales**: Se declaran variables globales que se utilizarán a lo largo del programa, como imágenes del juego, posición del jugador (Pac-Man), vidas, entre otros.

3. **Definición de funciones**: Se definen funciones para diversas tareas, como inicializar la pantalla, dibujar el mapa, dibujar el personaje, manejar la lógica del juego, entre otras. Estas funciones incluyen la función principal `main()`.

4. **Estructuras de datos para el mapa y los fantasmas**: Se define una estructura de datos para representar el mapa del juego, así como una clase para los fantasmas que incluye métodos para dibujarlos, moverlos y verificar colisiones con el jugador.

5. **Implementación del algoritmo AStar**: Aunque no se muestra en el código proporcionado, se menciona en los comentarios la inclusión de un archivo de encabezado `<AStar.h>` que podría contener la implementación del algoritmo de búsqueda A*. Este algoritmo probablemente se utiliza para el movimiento de los fantasmas en el juego.

6. **Función principal `main() `**: Aquí se encuentra la función principal del programa donde se realiza la inicialización de Allegro, se crea el buffer de pantalla, se carga el mapa, se ejecuta el bucle principal del juego y se gestionan los eventos del teclado.

En resumen, el código implementa un juego básico de Pac-Man utilizando la biblioteca Allegro en C++.

⚙️ Instalación y Ejecución
Clona este repositorio en tu máquina local.
Deberas instalar los componenetes necesarios para que allegro se ejecute correctamente, cabe recalcar que allgero se ejecuta correctamente en codeblocks version 17.12.
