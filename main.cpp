#include <allegro.h>
#include <stdio.h>
#include <AStar.h>
#include <Matriz.h>
//incluimos esta libreria para valores randomicos
#include <cstdlib>
#define MAXFILAS 50
#define MAXCOL 50

using namespace std;

FONT *mi_fuente; // Declaracion global

char nombre_jugador[50];


//Creamos el buffer donde estara el mapa
BITMAP *buffer;
//Creamos el muro del nivel
BITMAP *roca;
//Creamos  el pacman
BITMAP *pacbmp;
BITMAP *pacman;
//creamos o declaramos la comida
BITMAP *comida;
//Debemos crear cuando muere pacman
BITMAP *muertebmp;
// Variables de las vidas del pacman
BITMAP *vida1;
BITMAP *vida2;
BITMAP *vida3;
//PAra el mensaje de las vidas
BITMAP *mensaje;
//implementaremos la musica
//implementamos la musica para juego
SAMPLE* musica1;
//implementamos la musica cuando vaya comiendo lla comida
SAMPLE *bolita;
//implementamos la musica para cuando muera el pacman
SAMPLE *muerte;


//debemos crear una variale direccion para poder observar cuando vaya a la oozquierda o derecha

int dir= 4;//le ponemos direccion 4 para que este quieto
//debemos darle la posicion donde empezara pacman

int px = 30*18;
int py = 30*18;


//Debemos crear una posicion anterior de la que tiene pacman
int anterior_px;
int anterior_py;

// Variable para llevar la cuenta de las vidas
int vidas = 3;


void mostrar_cargando(BITMAP *buffer, int segundos) {
    int contador = 0;
    int progreso = 0;
    int paso = 1000; // Paso de actualización en milisegundos

    while (contador <=segundos * 1000) {
        clear(buffer);
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 2 - text_height(font) / 2,
                             makecol(255, 255, 255), -1, "Cargando... %d%%", progreso);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        rest(paso);
        contador += paso;
        progreso = (contador * 100) / (segundos * 1000);
    }
}

//Funcion para ingresar el nombre
void solicitar_nombre(BITMAP *buffer) {

    // Dibujar el resto de la pantalla

    // Calcular la longitud del texto "Por favor, ingresa tu nombre:"
        int texto_longitud = text_length(font, "Por favor, ingresa tu nombre:");

        // Definir el factor de escala para la línea
        float escala_linea = 0.8; // Ajusta este valor según sea necesario

        // Calcular la longitud de la línea relativa al texto
        int linea_longitud = static_cast<int>(texto_longitud * escala_linea);

        // Calcular las coordenadas de inicio y fin de la línea horizontal
        int linea_inicio = (1230 / 2) - (linea_longitud / 2);
        int linea_fin = (1230 / 2) + (linea_longitud / 2);

        // Actualizar el buffer con el nombre temporal
        clear(buffer);
        rectfill(buffer, 100, 200, 1130, 400, makecol(0, 0, 0)); // Rectángulo de fondo
        textout_centre_ex(buffer, font, "Bienvenido a Pacman!", 1230 / 2, 220, makecol(255, 255, 255), -1); // Mensaje de bienvenida
        textout_centre_ex(buffer, font, "Por favor, ingresa tu nombre:", 1230 / 2, 270, makecol(255, 255, 255), -1); // Mensaje de solicitud de nombre
        hline(buffer, linea_inicio, 310, linea_fin, makecol(255, 255, 255)); // Línea de separación centrada

    // Actualizar el buffer
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    char nombre_temporal[50] = {0};
    int indice = 0;

    while(true){
        // Esperar a que el usuario presione una tecla
        int key = readkey();

        // Extraer el código ASCII y el scan code de la tecla presionada
        int ascii_code = key & 0xff;
        int scan_code = (key >> 8) & 0xff;

        // Si se presiona Enter, salimos del bucle
        if (ascii_code == 13) {
            break;
        }

        // Si se presiona una tecla alfabética o un espacio y aún no hemos alcanzado el límite de caracteres, agregamos la tecla al nombre temporal
        if (((ascii_code >= 'A' && ascii_code <= 'Z') || (ascii_code >= 'a' && ascii_code <= 'z') || ascii_code == ' ') && indice < 49) {
            nombre_temporal[indice++] = ascii_code;
            nombre_temporal[indice] = '\0'; // Añadir terminador nulo al final de la cadena
        }

        // Actualizar el buffer con el nombre temporal
        clear(buffer);
        rectfill(buffer, 100, 200, 1130, 400, makecol(0, 0, 0)); // Rectángulo de fondo
        textout_centre_ex(buffer, font, "Bienvenido a Pacman!", 1230 / 2, 220, makecol(255, 255, 255), -1); // Mensaje de bienvenida
        textout_centre_ex(buffer, font, "Por favor, ingresa tu nombre:", 1230 / 2, 270, makecol(255, 255, 255), -1); // Mensaje de solicitud de nombre
        hline(buffer, linea_inicio, 310, linea_fin, makecol(255, 255, 255)); // Línea de separación centrada
        textout_centre_ex(buffer, font, nombre_temporal, 1230 / 2, 340, makecol(255, 255, 255), -1); // Nombre temporal
        blit(buffer, screen, 0, 0, 0, 0, 1230, 600);
    }

    // Copiar el nombre temporal en la variable nombre_jugador
    strcpy(nombre_jugador, nombre_temporal);

     // Llamamos a la función para mostrar el mensaje de carga durante 10 segundos
    mostrar_cargando(buffer, 10);
}





//mapa del nivel donde estara los muros
//en el mapa consideraremos a las X como el muro y las o como la comida del pacman


struct Coordenadas {
    int x;
    int y;

    Coordenadas(int _x, int _y) : x(_x), y(_y) {}
};


char mapa[MAXFILAS][MAXCOL]=
{
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX        ",
    "XoooooooooooooXoooooooooooooX       ",
    "XoXXXoXXXXXoXXXXXoXXXXXoXXXoX        ",
    "XoXXXoXXXXXoXXXXXoXXXXXoXXXoX       ",
    "XooFooooooooooXooooooooFooooX          ",
    "XoXXXoXXoXXXXXXXXXXXoXXoXXXoX     ",
    "XoooooXXoooooXXXoooooXXoooooX",
    "XoXXXoXXXXXX XXX XXXXXXoXXXoX      ",
    "XoXXXoXXoooooooooooooXXoXXXoX      ",
    " oooooXXoXXXXX|XXXXXoXXooooo        ",
    "XoXXXoXXoX    |    XoXXoXXXoX      ",
    "XoXXXoXXoXXXXXXXXXXXoXXoXXXoX      ",
    "XoXXXoXXoooooooooooooXXoXXXoX      ",
    "XoXXXoXXXXXXoXXXoXXXXXXoXXXoX      ",
    "XoooooXXoooooXXXoooooXXoooooX      ",
    "XoXXXoXXoXXXXXXXXXXXoXXoXXXoX      ",
    "XoXXXoooooooooooFoooooooXXXoX      ",
    "XoXXXoXXXXoXXXXXXXXoXXXoXXXoX      " ,
    "XoXXXoXXXXooooooooooXXXoXXXoX      ",
    "XooooooooooXXXXXXXXoooooooooX      ",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX      ",
};

void dibujar_mensaje(){
    draw_sprite(buffer,mensaje,900,475);//dibujamos el mensaje

}
// Función para dibujar las vidas del pacman
void dibujar_vidas() {
    if (vidas >= 1) {
        draw_sprite(buffer, vida1, 900, 530); // Dibujar primera vida
    }
    if (vidas >= 2) {
        draw_sprite(buffer, vida2, 960, 530); // Dibujar segunda vida
    }
    if (vidas >= 3) {
        draw_sprite(buffer, vida3, 1030, 530); // Dibujar tercera vida
    }
}
// Función para verificar el fin de juego
bool verificar_fin_de_juego() {
    return vidas <= 0; // Devuelve verdadero si el contador de vidas es igual o menor que cero
}
// Función para mostrar el mensaje de fin de juego
void mostrar_mensaje_de_fin_de_juego() {
    cout << "¡Fin del juego! Te has quedado sin vidas.\n";
}



int puntuacion = 0; // Variable para almacenar la puntuacion

// Funcion para actualizar la puntuacion
void actualizar_puntuacion(int puntos) {
    puntuacion += puntos;
}

void mostrar_game_over() {
    // Crear un buffer para dibujar
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);

    // Dibujar el fondo de Game Over
    rectfill(buffer, 0, 0, SCREEN_W, SCREEN_H, makecol(0, 0, 0));
    textout_centre_ex(buffer, font, "Game Over", SCREEN_W / 2, SCREEN_H / 2 - 40, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, "Puntuacion:", SCREEN_W / 2, SCREEN_H / 2, makecol(255, 255, 255), -1);
    textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 2 + 20, makecol(255, 255, 255), -1, "%d", puntuacion);
    textout_centre_ex(buffer, font, "Jugador:", SCREEN_W / 2, SCREEN_H / 2 + 40, makecol(255, 255, 255), -1);
    textout_centre_ex(buffer, font, nombre_jugador, SCREEN_W / 2, SCREEN_H / 2 + 60, makecol(255, 255, 255), -1);

    // Mostrar el buffer en la pantalla
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    // Esperar un poco antes de cerrar
    rest(5000); // 5000 milisegundos (5 segundos)

    // Liberar recursos
    destroy_bitmap(buffer);
}


// Función para eliminar una vida del pacman y verificar el fin del juego
void perder_vida() {
    if (vidas > 0) {
        vidas--; // Decrementar contador de vidas
        clear(buffer); // Limpiar buffer

        // Verificar si el juego ha terminado
        if (verificar_fin_de_juego()) {
            mostrar_mensaje_de_fin_de_juego();
            mostrar_game_over();
            rest(1000); // Esperar 1 segundos antes de salir del juego
            exit(0);
        }
    }
}




//Funcion para dibujar el mapa
void dibujar_mapa() {
    int row, col;

    for(row = 0; row < MAXFILAS; row++) {
        for (col = 0; col < MAXCOL; col++) {
            if (mapa[row][col] == 'X') {
                draw_sprite(buffer, roca, col * 30, row * 30);
            } else if (mapa[row][col] == 'o') {
                draw_sprite(buffer, comida, col * 30, row * 30);
                if (py / 30 == row && px / 30 == col) {
                    mapa[row][col] = ' ';
                }
            }
        }
    }




}


//Funcion para mostrar la puntuacion en la pantalla
void mostrar_puntuacion(BITMAP *buffer) {

    // Encontrar la posicion del ultimo muro en el mapa
    int ultima_columna_muro = 0;
    for (int fila = 0; fila < MAXFILAS; fila++) {
        for (int columna = 0; columna < MAXCOL; columna++) {
            if (mapa[fila][columna] == 'X') {
                if (columna > ultima_columna_muro) {
                    ultima_columna_muro = columna;
                }
            }
        }
    }

    // Calcular el ancho de la parte derecha de la pantalla
    int ancho_parte_derecha = SCREEN_W - (ultima_columna_muro + 1) * 30;

    // Llenar el area derecha de la pantalla con un rectangulo negro
    rectfill(buffer, (ultima_columna_muro + 1) * 30 + ancho_parte_derecha, 0, SCREEN_W, SCREEN_H, makecol(0, 0, 0));

    // Mostrar la puntuacion y el nombre del jugador en blanco sobre el fondo negro
    textprintf_right_ex(buffer, font, SCREEN_W - 20, 40, makecol(255, 255, 255), -1, "JUGADOR: %s", nombre_jugador);
    textprintf_right_ex(buffer, font, SCREEN_W - 20, 20, makecol(255, 255, 255), -1, "PUNTUACION: %d", puntuacion);
    // Actualizar el buffer en la pantalla
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

//Funcion para inicializar la pantalla
void pantalla()
{
    blit(buffer, screen, 0,0,0,0,1120,600);

}
//funcion para dibujar al pacman, pintar a pacman
void dibujar_personaje()
{
    //imprimir el personaje
    blit(pacbmp,pacman,dir*33,0,0,0,33,33); //lo que hacemos es insertar la posicion la imagen segun la imagen
    //mandamos a dibujar al pacman
    draw_sprite(buffer,pacman,px,py); //debemos poner donde tendremos al personaje


}

//funcion gameover para cuando pacman muera
bool game_over()
{
    //necesitamos columnas y filas definirkas
    int row, col;
    for(row=0; row<MAXFILAS; row++)
    {

        for (col  = 0 ; col < MAXCOL ; col++)
        {
            //aplicamos la condicion en donde alla una X ponda la imagen del muro
            if (mapa[row][col] == 'o' )
                return true; //si regresa verdad aun hay comida en el mapa
        }
    }
    //nos regresa que ya no hay comida
    return false;
}

//LO que haremos a continuacion sera crear la clase fantasma
class fantasma
{

    //Creamos el bit que sera el bufer
    BITMAP *enemigobmp;
//Creamps el fantasma
    BITMAP *enemigo;
    BITMAP *fantasAzul;
    BITMAP *fantasmaAzulbmp;
    //debemos crear una imagen para cada fantasma
    //Para el fantasma rojo
    BITMAP *fantasmaRojo;
    //Para el fantasma amarillo
    BITMAP *fantasmaAmarillo;
    //Para el fantasma celeste
    BITMAP *fantasmaCeleste;
    //Para el fantasma celeste
    BITMAP *fantasmaRosa;

    //Color del fantasma
    string color;

//creamos la direccion para los fantasmas
    int fdir ;
//coordenada de los fantasmas
    int _x ;
    int _y ;
//creamos un construcctor para poder inicializar todas las variables
public:
    fantasma(int x, int y,string color);
    //ingresamos las funciones que usamos
    //implementamos constante para cuando haya un cambio
    void dibujar_fantasma() const ;
    void mover_fantasma();
    void choque_pacman();
    void Buscarpacman_fantasma(int x,int y);
  void set_color(string new_color);

};
//creamos el constructos para darle los parametros
fantasma::fantasma(int x, int y,string color)
{
    _x = x;
    _y = y;
    fdir = rand()%4;
    //implementamos la imagen del fantasma
    //debemos medir la dimension de la imagen que es 30x30
    enemigo = create_bitmap(30,30);
    //creamos la imagen del fantasma
    enemigobmp = load_bitmap("enemigo.bmp",NULL);
     //Implementamos la imagen que es 30 x 31
    //Imagen para el fantasma rojo
     fantasmaRojo = create_bitmap(30, 31);
    fantasmaRojo = load_bitmap("fantasmarojo.bmp", NULL);
    //Imagen para el fantasma amarillo
    fantasmaAmarillo = create_bitmap(30, 30);
    fantasmaAmarillo = load_bitmap("fantasmaamarillo.bmp", NULL);
    //Imagen para el fantasma celeste
    fantasmaCeleste = create_bitmap(30, 31);
    fantasmaCeleste = load_bitmap("fantasmaceleste.bmp", NULL);
    //Imagen para el fantasma rosa
    fantasmaRosa= create_bitmap(30, 28);
    fantasmaRosa= load_bitmap("fantasmarosa.bmp", NULL);


}
//realizamos las funciones
//funcion para dibujar al fantasma
void fantasma::dibujar_fantasma() const
{
//imprimir el personaje
    if (color == "rojo")
    blit(fantasmaRojo, enemigo, 0, 0, 0, 0, 30, 30);
    else if (color == "amarillo")
        blit(fantasmaAmarillo, enemigo, 0, 0, 0, 0, 30, 30);
    else if (color == "celeste")
        blit(fantasmaCeleste, enemigo, 0, 0, 0, 0, 30, 30);
    else if (color == "rosa")
        blit(fantasmaRosa, enemigo, 0, 0, 0, 0, 30, 30);

        else // Si no se especifica un color válido, usamos la imagen predeterminada
            blit(enemigobmp, enemigo, 0, 0, 0, 0, 30, 30);
    //mandamos a dibujar al fantasma
    draw_sprite(buffer,enemigo,_x,_y); //debemos poner donde tendremos al personaje
}

//creamos una funcion para cuando haya un choque o colision del fantasma con pacman
void fantasma::choque_pacman()
{
    //revisamos si existe la misma posicion del fantasma con el pacman
    if (py == _y  && px == _x || _y == anterior_py && _x == anterior_px)
    {
        //creamos un bucle for para que se vea el proceso de eliminar el pacman
        for (int j = 0; j<= 5; j++)
        {
            //hacer esta funcion para que desaparezca el pacman
            clear(pacman);
            //limpiamos el buffer
            clear(buffer);
            //mandamos a que se dibuje de nuevo el mapa
            dibujar_mapa();
            //ingresamos la ilustracion de la muerte
            blit(muertebmp,pacman,j*33,0,0,0,33,33);
            //insertamos en el buffer mismo de la posicion
            draw_sprite(buffer,pacman,px,py);
            //para que todo esto se vea hay que volver a llamar a la pantalla
            pantalla();
            //damos un tiempo
            rest(80);
        }
        //llamamos a la funcion perder_vida para la perdida de vida del pac
        perder_vida();
        px = 30*18;
        py = 30*18;
        dir = 4;
    }


}

//con esta funcion dibujamos al fantasma y hacemos la rutima para mover al fantasma

void fantasma::mover_fantasma()
{


//mandamos a ilutrar al fantasma
    dibujar_fantasma();
    //implementamos la funcion del choque pacman
    choque_pacman();
    //Creamos una direccion en caso de que haya caminos en el medio
    if(mapa[_y/30][_x/30] == '|')
    {
        fdir = rand()%4;
    }

    //le daremos el movimiento al fantasma
    if(fdir == 0  )
    {
        //debemos declarar para evitar que pase sobre los muros
        if(mapa[_y/30][(_x-30)/30] != 'X')  //debemos fijar la posicion un paso mas a la izquierda
            _x -= 30 ; //Decremento a la posicion  para que se mueva a la izquierda
        else
            fdir =rand()%4 ; //fantasma se movera ya sea arriba, abajo, izquierda o derecha
    }

    if (fdir == 1)
    {
        if(mapa[_y/30][(_x+30)/30] != 'X')  //debemos fijar la posicion un paso mas a la iderecha
            _x += 30; //incremento a la posicion para que se mueva a la derecha
        else
            fdir = rand( )%4; //fantasma se movera ya sea arriba, abajo, izquierda o derecha

    }
    if (fdir == 2)
    {
        if(mapa[(_y-30)/30][(_x)/30] != 'X')  //debemos fijar la posicion un paso mas para arriba
            _y -= 30; //decremento a la posicion en y para que vaya arriba
        else
            fdir = rand( )%4; //fantasma se movera ya sea arriba, abajo, izquierda o derecha

    }
    if (fdir == 3)
    {
        if(mapa[(_y+30)/30][(_x)/30] != 'X')  //debemos fijar la posicion un paso mas para arriba
            _y += 30; //incremento a la posicion en y para que vaya abajo
        else
            fdir = rand( )%4 ; //fantasma se movera ya sea arriba, abajo, izquierda o derecha
    }

    //debemos hacer la rutina para los atajos
    //debemos crear una direcion para cuando tome atajos
    if(_x <= -30)
        _x = 870; //atajo de la izquierda
    else if (_x >= 870)
        _x = -30;


}
void fantasma::Buscarpacman_fantasma(int x,int y)
{
    _x=x;
    _y=y;
    //mandamos a ilutrar al fantasma
    dibujar_fantasma();
    //implementamos la funcion del choque pacman
    choque_pacman();
    //Creamos una direccion en caso de que haya caminos en el medio


    //debemos hacer la rutina para los atajos
    //debemos crear una direcion para cuando tome atajos
    if(_x <= -30)
        _x = 870; //atajo de la izquierda
    else if (_x >= 870)
        _x = -30;


}

int main ()
{
    int i=0;
    int finalFY=0;
    int finalFX=0;
    int FCfinalFY=0;
    int FCfinalFX=0;


   // Grafo del mapa Pacman
                 //1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29
                 //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28

        vector<vector<int> > grid;
                 //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28
    int row1[] =  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int row2[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    int row3[] =  {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    int row4[] =  {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    int row5[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    int row6[] =  {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    int row7[] =  {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1};
    int row8[] =  {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    int row9[] =  {1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    int row10[] = {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0};
    int row11[] = {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    int row12[] = {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    int row13[] = {1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    int row14[] = {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    int row15[] = {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1};
    int row16[] = {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1};
    int row17[] = {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1};
    int row18[] = {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    int row19[] = {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    int row20[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    int row21[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};



    grid.push_back(vector<int>(row1, row1 + sizeof(row1) / sizeof(row1[0])));
    grid.push_back(vector<int>(row2, row2 + sizeof(row2) / sizeof(row2[0])));
    grid.push_back(vector<int>(row3, row3 + sizeof(row3) / sizeof(row3[0])));
    grid.push_back(vector<int>(row4, row4 + sizeof(row4) / sizeof(row4[0])));
    grid.push_back(vector<int>(row5, row5 + sizeof(row5) / sizeof(row5[0])));
    grid.push_back(vector<int>(row6, row6 + sizeof(row6) / sizeof(row6[0])));
    grid.push_back(vector<int>(row7, row7 + sizeof(row7) / sizeof(row7[0])));
    grid.push_back(vector<int>(row8, row8 + sizeof(row8) / sizeof(row8[0])));
    grid.push_back(vector<int>(row9, row9 + sizeof(row9) / sizeof(row9[0])));
    grid.push_back(vector<int>(row10, row10 + sizeof(row10) / sizeof(row10[0])));
    grid.push_back(vector<int>(row11, row11 + sizeof(row11) / sizeof(row11[0])));
    grid.push_back(vector<int>(row12, row12 + sizeof(row12) / sizeof(row12[0])));
    grid.push_back(vector<int>(row13, row13 + sizeof(row13) / sizeof(row13[0])));
    grid.push_back(vector<int>(row14, row14 + sizeof(row14) / sizeof(row14[0])));
    grid.push_back(vector<int>(row15, row15 + sizeof(row15) / sizeof(row15[0])));
    grid.push_back(vector<int>(row16, row16 + sizeof(row16) / sizeof(row16[0])));
    grid.push_back(vector<int>(row17, row17 + sizeof(row17) / sizeof(row17[0])));
    grid.push_back(vector<int>(row18, row18 + sizeof(row18) / sizeof(row18[0])));
    grid.push_back(vector<int>(row19, row19 + sizeof(row19) / sizeof(row19[0])));
    grid.push_back(vector<int>(row20, row20 + sizeof(row20) / sizeof(row20[0])));
    grid.push_back(vector<int>(row21, row21 + sizeof(row21) / sizeof(row21[0])));
    AStar astar(grid);
    /*Filas*/int startY = 10;
    /*Columnas*/int startX = 10;
    /*Filas*/int targetY = px/30;
    /*Columnas*/int targetX =py/30 ;

        finalFY=targetY;
        finalFX=targetX;


    vector<pair<int, int> > path = astar.findPath(startX, startY, targetX, targetY);


    Matriz<int> Camino(1000, 2);

    Camino(1, 1);
    if (!path.empty()) {
        cout << "Camino encontrado Fantasma A:" << endl;
        for (size_t i = 0; i < path.size(); ++i) {
            Camino(i, 0)=path[i].first;
            Camino(i, 1)=path[i].second;
            cout << i<<" (" << Camino(i, 1)<< ", " << Camino(i, 0)<< ")" << endl;
        }

    } else {
        cout << "No se encontró un camino." << endl;
    }


    /*Filas*/int FCstartY = 20;
    /*Columnas*/int FCstartX = 10;
    /*Filas*/int FCtargetY = px/30;
    /*Columnas*/int FCtargetX =py/30 ;

        FCfinalFY=FCtargetY;
        FCfinalFX=FCtargetX;


    vector<pair<int, int> > FCpath = astar.findPath(startX, startY, targetX, targetY);

    Matriz<int> FCCamino(1000, 2);

    FCCamino(1, 1);
    if (!path.empty()) {
        cout << "Camino encontrado Fantasma C:" << endl;
        for (size_t i = 0; i < FCpath.size(); ++i) {
            FCCamino(i, 0)=FCpath[i].first;
            FCCamino(i, 1)=FCpath[i].second;
            cout << i<<" (" << FCCamino(i, 1)<< ", " << FCCamino(i, 0)<< ")" << endl;
        }

    } else {
        cout << "No se encontró un camino." << endl;
    }

    /* *** Con estas lineas de codigo preparamos el entorno para graficos y sonidos *** */
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1120,600,0, 0);

    /* ******************************************************************************** */
    //debemos inicializar allegro para tener o usar sonidos
     if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }
    //ajustamos ell volumen
    set_volume(70, 70);

    //ingresamos la musica
    bolita = load_wav("pacman_chomp.wav"); //esta musica nos sirve cuando el pacman coma una bolita
    musica1 = load_wav("pacman_beginning.wav"); //Esta musica nos sirve para darle sonido al juego
    muerte = load_wav("pacman_death.wav");//cuando muere pacman
    //Creada del buffer parael mapa
    buffer = create_bitmap(1230,600);
    //creada e insertar la imagen del muro
    roca = load_bitmap("roca.bmp", NULL);
    //creamos la ilustracion de la imagen
    pacbmp = load_bitmap("pacman.bmp",NULL);

    solicitar_nombre(buffer); // Llamar a la funciOn para solicitar el nombre antes de comenzar el juego


    //peque;o buffer
    //debemos medir la dimension de la imagen que es 33x33
    pacman = create_bitmap(33,33);
    comida = load_bitmap("Comida.bmp",NULL);

    //Ingresamos la ilustracion de la muerte de pacman
    muertebmp = load_bitmap("muerte.bmp",NULL);
      // Cargar imágenes de las vidas del pacman

    vida1 = load_bitmap("lifepac.bmp", NULL);
    vida2 = load_bitmap("lifepac.bmp", NULL);
    vida3 = load_bitmap("lifepac.bmp", NULL);

    //Cargar mensaje vida
    mensaje=load_bitmap("vidasMensaje.bmp",NULL);
   //invocamos al constructor fantasma
    fantasma A(30*10,30*10,"rojo");
    fantasma B(30*14, 30*8,"amarillo");
    fantasma C (30*18, 30*10,"celeste");
    fantasma D(30*10, 30*10,"rosa");

    int j=0;
    int k=0;
    int TmF1=0;
    int TmF2=0;
    int TmF3=0;
    int TmF4=0;
    int T=0;

    //condicion de while que se ejecutara hasta que se presione la tecla de escape
    while(!key[KEY_ESC] && game_over())
    {
        T++;

        //insertamos los movimientos anteriores
        anterior_px = px;
        anterior_py = py;

        //necesitamos mover al pacman , aplicamos la variable direccion
        if (key[KEY_RIGHT])
            dir = 1;
        else if (key[KEY_LEFT])
            dir = 0;
        else if (key[KEY_UP])
            dir = 2;
        else if (key[KEY_DOWN])
            dir = 3;

        //Vamos a mover al pacman

        if(dir == 0  )
        {
            //debemos declarar para evitar que pase sobre los muros
            if(mapa[py/30][(px-30)/30] != 'X')  //debemos fijar la posicion un paso mas a la izquierda
                px -= 30 ; //Decremento a la posicion  para que se mueva a la izquierda
            else
                dir =4 ; //pacman no se movera ya que no hay una opcion para que se mueva
        }

        if (dir == 1)
        {
            if(mapa[py/30][(px+30)/30] != 'X')  //debemos fijar la posicion un paso mas a la iderecha
                px += 30; //incremento a la posicion para que se mueva a la derecha
            else
                dir =4 ; //pacman no se movera ya que no hay una opcion para que se mueva

        }
        if (dir == 2)
        {
            if(mapa[(py-30)/30][(px)/30] != 'X')  //debemos fijar la posicion un paso mas para arriba
                py -= 30; //decremento a la posicion en y para que vaya arriba
            else
                dir =4 ; //pacman no se movera ya que no hay una opcion para que se mueva

        }
        if (dir == 3)
        {
            if(mapa[(py+30)/30][(px)/30] != 'X')  //debemos fijar la posicion un paso mas para arriba
                py += 30; //incremento a la posicion en y para que vaya abajo
            else
                dir =4 ; //pacman no se movera ya que no hay una opcion para que se mueva
        }

        //debemos crear una direcion para cuando tome atajos
        if(px <= -30)
            px = 870; //atajo de la izquierda
        else if (px >= 870)
            px = -30;
        clear(buffer); //mandamos a que se limpie los rastros que deja el pacman

        dibujar_mapa();
        dibujar_personaje();

        // Detectar si el pacman come una bolita después de moverse
        int nueva_px = px;
        int nueva_py = py;

        // Calcular las coordenadas de la casilla actual del pacman en el mapa
        int casilla_x = nueva_px / 30;
        int casilla_y = nueva_py / 30;

        // Verificar si la casilla actual del pacman contiene una bolita ('o')
        if (mapa[casilla_y][casilla_x] == 'o') {
            // Incrementar la puntuación
            actualizar_puntuacion(10); // Aumentamos en 10 puntos por cada bolita comida

            // Cambiar la posición de la bolita en el mapa a un espacio en blanco
            mapa[casilla_y][casilla_x] = ' ';
        }

        cout<<"PUNTACION: "<<puntuacion<<endl;

        mostrar_puntuacion(buffer); // Llamar a la funcion para mostrar la puntuacion

        if(T==0){
            TmF1=T;
        }
        if(TmF1==0){

            cout << "Final "<<i<<" (" << finalFX << ", " << finalFY<< ")" << endl;
            cout << "Camm "<<i<<" (" << Camino(i, 1)<< ", " << Camino(i, 0)<< ")" << endl;
            if( finalFX==Camino(i, 1) && finalFY==Camino(i, 0)){
                path.clear();
                 /*Filas*/int startXNew=Camino(i, 0) ;

                /*Columnas*/int startYNew = Camino(i, 1);
                /*Filas*/  int targetXNew = py/30;
                /*Columnas*/ int targetYNew= px/30;


                vector<pair<int, int> >Newpath = astar.findPath(startXNew, startYNew, targetXNew, targetYNew);
                cout <<Camino(i, 1)<<","<<Camino(i, 0)<<endl;
                Matriz<int> Camino1(Newpath.size(), 2);
                if (!Newpath.empty()) {
                cout << "Camino encontrado Fantasma A:" << endl;
                for (j = 0; j < Newpath.size(); ++j) {
                Camino1(j, 0)=Newpath[j].first;
                Camino1(j, 1)=Newpath[j].second;
                Camino(j, 1)= Camino1(j, 1);
                Camino(j, 0)= Camino1(j, 0);
                cout << j<<" (" <<Camino(j, 1)<< ", " << Camino(j, 0)<< ")" << endl;
                }
                finalFY=Camino(j-1, 0);
                finalFX=Camino(j-1, 1);
                ;
               }
            i=0;
            }
            cout << "Fantasma A:"<<i<<" (" << Camino(i, 1)<< ", " << Camino(i, 0)<< ")" << endl;
            A.Buscarpacman_fantasma(Camino(i, 1)*30,Camino(i, 0)*30);
            i++;
            cout <<px/30<<","<<py/30<< endl;
        }
        if(T==60){
            TmF2=T;
        }
         if(TmF2==60){
            B.mover_fantasma();
         }
        if(T==80){
            TmF3=T;
        }
         if(TmF3==80){
            cout << "Tar "<<k<<" (" << FCtargetY << ", " << FCtargetX << ")" << endl;
            cout << "Final "<<k<<" (" << FCfinalFX << ", " << FCfinalFY<< ")" << endl;
            cout << "Camm "<<k<<" (" << FCCamino(k, 1)<< ", " << FCCamino(k, 0)<< ")" << endl;
            if( FCfinalFX==FCCamino(k, 1) && FCfinalFY==FCCamino(k, 0)){
                path.clear();
                 /*Filas*/int FCstartXNew=FCCamino(k, 0) ;

                /*Columnas*/int FCstartYNew = FCCamino(k, 1);
                /*Filas*/  int FCtargetXNew = py/30;
                /*Columnas*/ int FCtargetYNew= px/30;



                vector<pair<int, int> >FCNewpath = astar.findPath(FCstartXNew, FCstartYNew, FCtargetXNew, FCtargetYNew);

                cout <<FCCamino(k, 1)<<","<<FCCamino(k, 0)<<endl;

                Matriz<int> FCCamino1(FCNewpath.size(), 2);
                if (!FCNewpath.empty()) {
                cout << "Camino encontrado Fantasma C:" << endl;
                for (j = 0; j < FCNewpath.size(); ++j) {
                FCCamino1(j, 0)=FCNewpath[j].first;
                FCCamino1(j, 1)=FCNewpath[j].second;
                FCCamino(j, 1)= FCCamino1(j, 1);
                FCCamino(j, 0)= FCCamino1(j, 0);
                cout << j<<" (" <<FCCamino(j, 1)<< ", " << FCCamino(j, 0)<< ")" << endl;
                }
                FCfinalFY=FCCamino(j-1, 0);
                FCfinalFX=FCCamino(j-1, 1);
                ;
               }
            k=0;
            }
            cout << "Fantasma C: "<<k<<" (" << FCCamino(i, 1)<< ", " << FCCamino(i, 0)<< ")" << endl;
            C.Buscarpacman_fantasma(FCCamino(k, 1)*30,FCCamino(k, 0)*30);
            k++;
            cout <<px/30<<","<<py/30<< endl;
         }

        if(T==100){
            TmF4=T;
        }
         if(TmF4==100){
            D.mover_fantasma();
         }

        dibujar_mensaje();
        dibujar_vidas();

        /*if (game_over()) {
            perder_vida(); // Llamar a la función para reducir el contador de vidas
            rest(5000); // Esperar 5 segundos antes de salir del juego
            break;
        }*/

        pantalla();
        //darle un tiempo al programa para poder ver el tiempo de pacman
        rest(100);//7- milisegundos para esperar
        clear(pacman); //mandamos a que se limpie el pacman para que nos de la ultima posicion de la imagen
        //imprimir el personaje
        blit(pacbmp,pacman,4*33,0,0,0,33,33); //lo que hacemos es insertar la posicion la imagen segun la imagen
        //mandamos a dibujar al pacman
        draw_sprite(buffer,pacman,px,py); //debemos poner donde tendremos al personaje
        //volvemos a mandar la pantalla
        pantalla();
        //volvemos a darle un tiempo para el personaje
        rest(100);


    }


    return 0;
}
END_OF_MAIN ()




