#include <iostream>
#include <vector>
#include <queue>//Cola prioritaria
#include <cmath>
#include <algorithm>

using namespace std;

class AStar {
private:
    // Definición de una estructura para representar un nodo en el grafo
    struct Node {
        int x, y; // Coordenadas del nodo
        double cost; // Costo actual para llegar a este nodo
        double heuristic; // Valor heuristico para este nodo

        Node(int _x, int _y, double _cost, double _heuristic) : x(_x), y(_y), cost(_cost), heuristic(_heuristic) {}


        bool operator<(const Node &other) const {
            return (cost + heuristic) > (other.cost + other.heuristic); // compara dos nodos, el actual y otro nodo (other)
            //retorna TRUE si el nodo actual tiene una prioridad menor que el otro nodo, y FAlSE en caso contrario
        }
    };

    // Función para calcular la distancia Euclidiana entre dos punto
    //Pitagoras
    double euclideanDistance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

public:
    AStar(const vector<vector<int> >& grid) : grid(grid) {}

    vector<pair<int, int> > findPath(int startX, int startY, int targetX, int targetY) {
        int numRows = grid.size();//NUmero de Filas de la matriz del grafo
        int numCols = grid[0].size();//Numero de Columnas de la matriz del grafo

        // Direcciones posibles: arriba, abajo, izquierda, derecha
        //-1 movimiento hacia la izquierda y 1 el movimiento hacia la derecha.
        int dx[] = {-1, 1, 0, 0};//desplazamientos horizontales posibles
        //-1 movimiento hacia arriba y 1 el movimiento hacia abajo
        int dy[] = {0, 0, -1, 1};//contiene los desplazamientos verticales posibles

        // Inicialización de la para almacenar los nodos a visitar.cola ordena de menor a mayos los nodos ingresados
        priority_queue<Node> pq;
        // calcula la distancia euclidiana entre el nodo inicial y el nodo objetivo
        pq.push(Node(startX, startY, 0.0, euclideanDistance(startX, startY, targetX, targetY)));


        //Inicializa cada fila de la matriz parent con un vector de pares de enteros con (-1, -1)
        vector<vector<pair<int, int> > > parent(numRows, vector<pair<int, int> >(numCols, make_pair(-1, -1)));

        //Cada celda de esta matriz bidimencional almacenará el costo acumulado para llegar a ese nodo desde el nodo inicial

        vector<vector<double> > cost(numRows, vector<double>(numCols, INFINITY));
        cost[startX][startY] = 0.0;

        // Bucle principal del algoritmo A*
        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();

            int x = current.x;
            int y = current.y;

            // Explorar los nodos adyacentes
            for (int i = 0; i < 4; ++i) {
                int newX = x + dx[i];
                int newY = y + dy[i];

                // Verificar si el nuevo nodo está dentro de los límites del grid y es transitable
                if (newX >= 0 && newX < numRows && newY >= 0 && newY < numCols && grid[newX][newY] == 0) {
                    double newCost = cost[x][y] + 1.0; // Costo uniforme (1 por movimiento)
                    if (newCost < cost[newX][newY]) {
                        cost[newX][newY] = newCost;
                        parent[newX][newY] = make_pair(x, y);
                        pq.push(Node(newX, newY, newCost, euclideanDistance(newX, newY, targetX, targetY)));
                    }
                }
            }

            // Si llegamos al nodo objetivo, construimos el camino de regreso
            if (x == targetX && y == targetY) {
                vector<pair<int, int> > path;
                while (x != startX || y != startY) {
                    path.push_back(make_pair(x, y));
                    int prevX = parent[x][y].first;
                    int prevY = parent[x][y].second;
                    x = prevX;
                    y = prevY;
                }
                path.push_back(make_pair(startX, startY));
                reverse(path.begin(), path.end());
                return path;
            }

        }

        // Si no se encuentra un camino, devolver un vector vacío
        return vector<pair<int, int> >();
    }

    vector<vector<int> > grid;
};


