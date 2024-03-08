#define MATRIZ.H
#include <iostream>
using namespace std;

template <typename T>
class Matriz {
private:
    T** elementos;
    int tamanio_fila, tamanio_columna;

public:
    Matriz(int filas, int columnas) {
        tamanio_fila = filas;
        tamanio_columna = columnas;

        elementos = new T*[tamanio_fila];
        for (int i = 0; i < tamanio_fila; ++i) {
            elementos[i] = new T[tamanio_columna];
        }

        // Inicializar todos los elementos a T()
        for (int i = 0; i < tamanio_fila; ++i) {
            for (int j = 0; j < tamanio_columna; ++j) {
                elementos[i][j] = T();
            }
        }
    }

    // Función asignar valores a los elementos de la matriz
    void entregarElemento(int fila, int columna, T elemento) {
        if (fila >= 0 && fila < tamanio_fila && columna >= 0 && columna < tamanio_columna) {
            elementos[fila][columna] = elemento;
        } else {
            cout << "Índices fuera de rango." << endl;
        }
    }

    //   permite acceder los elementos de la matriz
    T& operator()(int fila, int columna) {
        return elementos[fila][columna];
    }

};
