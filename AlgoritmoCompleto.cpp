#include <iostream>
#include <vector>

using namespace std;

const int N = 15; // Número de comunas
vector<int> costo = {60, 30, 60, 70, 130, 60, 70, 60, 80, 70, 50, 90, 30, 30, 100}; // Costos de construccion

// Matriz de cobertura 15x15 donde la fila i indica si la comuna i cubre a las comunas j (1 para cubrir, 0 para no cubrir)
vector<vector<int>> cobertura = {
//   1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 1 ESTÁ BIEN
    {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, // 2 ESTÁ BIEN
    {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 3 ESTÁ BIEN
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // 4 ESTÁ BIEN
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0}, // 5 ESTÁ BIEN
    {0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0}, // 6 ESTÁ BIEN
    {0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1}, // 7 ESTÁ BIEN
    {0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // 8 ESTÁ BIEN
    {0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0}, // 9 ESTÁ BIEN
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0}, // 10 ESTÁ BIEN
    {0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0}, // 11 ESTÁ BIEN
    {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1}, // 12 ESTÁ BIEN
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, // 13 ESTÁ BIEN
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1}, // 14 ESTÁ BIEN
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1}  // 15 ESTÁ BIEN
}; // Cobertura de cada comuna como matriz 15x15

vector<int> x(N, 0); // Vector para las decisiones (0 o 1), que comuna se construye o no
vector<vector<int>> soluciones; // Para almacenar todas las soluciones válidas
int mejor_costo = INT_MAX; // Para almacenar el costo de la mejor solucion

// Funcion para calcular el costo total
int costo_total() {
    int total = 0;
    for (int i = 0; i < N; ++i) {
        if (x[i] == 1) {
            total += costo[i];
        }
    }
    return total;
}

// Verifica si la comuna está cubierta por algún centro de vacunacion
bool es_cubierta(int comuna) {
    for (int i = 0; i < N; ++i) {
        if (x[i] == 1 && cobertura[i][comuna] == 1) {
            return true;
        }
    }
    return false;
}

// Funcion recursiva de backtracking
void backtracking(int idx) {
    if (idx == N) {  // Si todas las comunas han sido evaluadas
        bool todasCubiertas = true;
        for (int i = 0; i < N; ++i) {
            if (!es_cubierta(i)) { // Verificar que todas las comunas estén cubiertas
                todasCubiertas = false;
                break;
            }
        }

        if (todasCubiertas) {
            int costoActual = costo_total();
            if (costoActual < mejor_costo) {
                // Actualizar la mejor solucion
                mejor_costo = costoActual;
                soluciones.clear();
                soluciones.push_back(x); // Almacenar la solucion actual
            } else if (costoActual == mejor_costo) {
                soluciones.push_back(x); // Almacenar otra solucion con el mismo costo
            }
        }
        return;
    }

    // Probar asignando 1 (construir un centro en la comuna idx)
    x[idx] = 1;
    backtracking(idx + 1); // Recursion hacia el siguiente centro

    // Probar asignando 0 (no construir un centro en la comuna idx)
    x[idx] = 0;
    backtracking(idx + 1); // Recursion hacia el siguiente centro
}

int main() {
    backtracking(0);

    if (!soluciones.empty()) {
        cout << "Se encontraron soluciones!" << endl;
        cout << "Costo total de la mejor solucion: " << mejor_costo << endl;
        cout << "Las soluciones son:" << endl;
        for (const auto& sol : soluciones) {
            cout << "Comunas con centro de vacunacion: ";
            for (int i = 0; i < N; ++i) {
                if (sol[i] == 1) {
                    cout << i + 1 << " ";  // Comuna i+1
                }
            }
            cout << endl;
        }
    } else {
        cout << "No se encontro solucion." << endl;
    }

    return 0;
}
