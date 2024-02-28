#include <allegro.h>
#include <stdio.h>
//incluimos esta libreria para valores randomicos
#include <cstdlib>
#define MAXFILAS 20
#define MAXCOL 32

using namespace std;

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

int px = 30*17;
int py = 30*17;


//Debemos crear una posicion anterior de la que tiene pacman
int anterior_px;
int anterior_py;

//mapa del nivel donde estara los muros
//en el mapa consideraremos a las X como el muro y las o como la comida del pacman
char mapa[MAXFILAS][MAXCOL]=
{
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X  o |o o o XXXXX o o o| o  X",
    "X XXX XXXXX XXXXX XXXXX XXX X",
    "XoXXX XXXXX XXXXX XXXXX XXXoX",
    "X      o|o   o o   o|o      X",
    "XoXXXoXX XXXXXXXXXXX XXoXXXoX",
    "X    |XX    |XXX|    XX     X",
    "XoXXXoXXXXXX XXX XXXXXXoXXXoX",
    "X XXXoXX ooo|ooo|ooo XXoXXX X",
    " o   |XX XXXXXXXXXXX XX|   o ",
    "X XXXoXX XXXXXXXXXXX XXoXXX X",
    "XoXXXoXX oo |ooo|ooo XXoXXXoX",
    "X XXXoXXXXXX XXX XXXXXXoXXX X",
    "X     XX     XXX     XX     X",
    "X XXXoXX XXXXXXXXXXX XXoXXX X",
    "XoXXX| o| o o o o o |o |XXXoX",
    "X XXXoXXXX XXXXXXXX XXX XXX X",
    "XoXXXoXXXX          XXX XXXoX",
    "X  o |o o  XXXXXXXX o o| o  X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

//funcion para dibujar el mapa
void dibujar_mapa()
{
    //necesitamos columnas y filas definirkas
    int row, col;
    //creamos bucles para que recorra todo nuestro mapa
    for(row=0; row<MAXFILAS; row++)
    {

        for (col  = 0 ; col < MAXCOL ; col++)
        {
            //aplicamos la condicion en donde alla una X ponda la imagen del muro
            if (mapa[row][col] == 'X' )
            {
                //funcion para que se proyecte el muro en cada lugar donde este una X
                draw_sprite(buffer, roca, col*30, row*30);
            }
            else if(mapa[row][col] == 'o' )
            {
                //funcion para que se proyecte la comida en cada lugar donde este una X
                draw_sprite(buffer, comida, col*30, row*30);
                //necesitamos elimiar la comida cuando pase el pacman
                if (py/30 ==row && px/30 == col )
                {

                    //verificamos si pacman esta en esa posicion
                    mapa[row][col] = ' '; //no habra nada osea se borra


                }


            }
        }
    }
}
//Funcion para inicializar la pantalla
void pantalla()
{
    blit(buffer, screen, 0,0,0,0,880,600);
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

//creamos la direccion para los fantasmas
    int fdir ;
//coordenada de los fantasmas
    int _x ;
    int _y ;
//creamos un construcctor para poder inicializar todas las variables
public:
    fantasma(int x, int y);
    //ingresamos las funciones que usamos
    //implementamos constante para cuando haya un cambio
    void dibujar_fantasma() const ;
    void mover_fantasma();
    void choque_pacman();

};
//creamos el constructos para darle los parametros
fantasma::fantasma(int x, int y)
{
    _x = x;
    _y = y;
    fdir = rand()%4;
    //implementamos la imagen del fantasma
    //debemos medir la dimension de la imagen que es 30x30
    enemigo = create_bitmap(30,30);
    //creamos la imagen del fantasma
    enemigobmp = load_bitmap("enemigo.bmp",NULL);


}
//realizamos las funciones
//funcion para dibujar al fantasma
void fantasma::dibujar_fantasma() const
{
//imprimir el personaje
    blit(enemigobmp,enemigo,0,0,0,0,30,30); //lo que hacemos es insertar la posicion la imagen segun la imagen
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
        px = 30*17;
        py = 30*17;
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
int main ()
{
    /* *** Con estas lineas de codigo preparamos el entorno para graficos y sonidos *** */
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880,600,0, 0);

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
    buffer = create_bitmap(880,600);
    //creada e insertar la imagen del muro
    roca = load_bitmap("roca.bmp", NULL);
    //creamos la ilustracion de la imagen
    pacbmp = load_bitmap("pacman.bmp",NULL);

    //peque;o buffer
    //debemos medir la dimension de la imagen que es 33x33
    pacman = create_bitmap(33,33);
    comida = load_bitmap("Comida.bmp",NULL);

    //Ingresamos la ilustracion de la muerte de pacman
    muertebmp = load_bitmap("muerte.bmp",NULL);
   //invocamos al constructor fantasma
   fantasma A(30*3,30*4);
   fantasma B(30*10, 30*11);


    //condicion de while que se ejecutara hasta que se presione la tecla de escape
    while(!key[KEY_ESC] && game_over())
    {

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
        A.mover_fantasma();
        B.mover_fantasma();
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


//hola

