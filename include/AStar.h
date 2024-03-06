#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

class AStar {
public:
    AStar(const vector<vector<int> >& grid) : grid(grid) {}

    vector<pair<int, int> > findPath(int startX, int startY, int targetX, int targetY) {
        int numRows = grid.size();
        int numCols = grid[0].size();

        // Direcciones posibles: arriba, abajo, izquierda, derecha
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Inicialización de la cola de prioridad para almacenar los nodos a visitar
        priority_queue<Node> pq;
        pq.push(Node(startX, startY, 0.0, euclideanDistance(startX, startY, targetX, targetY)));

        // Vector para almacenar el nodo padre de cada nodo
        vector<vector<pair<int, int> > > parent(numRows, vector<pair<int, int> >(numCols, make_pair(-1, -1)));

        // Vector para almacenar el costo actual para llegar a cada nodo
        vector<vector<double> > cost(numRows, vector<double>(numCols, INFINITY));
        cost[startX][startY] = 0.0;

        // Bucle principal del algoritmo A*
        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();

            int x = current.x;
            int y = current.y;

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
        }

        // Si no se encuentra un camino, devolver un vector vacío
        return vector<pair<int, int> >();
    }

private:
    // Definición de una estructura para representar un nodo en el grafo
    struct Node {
        int x, y; // Coordenadas del nodo
        double cost; // Costo actual para llegar a este nodo
        double heuristic; // Valor heurístico para este nodo

        Node(int _x, int _y, double _cost, double _heuristic) : x(_x), y(_y), cost(_cost), heuristic(_heuristic) {}

        // Sobrecarga del operador "<" para comparar nodos basado en su costo total
        bool operator<(const Node &other) const {
            return (cost + heuristic) > (other.cost + other.heuristic);
        }
    };

    // Función para calcular la distancia Euclidiana entre dos puntos
    double euclideanDistance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    vector<vector<int> > grid;
};


