#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <numeric>
#include <cmath>

using namespace std;
using namespace std::chrono;

const int N = 15;
vector<int> costo = {60, 30, 60, 70, 130, 60, 70, 60, 80, 70, 50, 90, 30, 30, 100};
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
    };

vector<int> x(N, 0);
vector<vector<int>> soluciones;
int mejor_costo = INT_MAX;
int nodos_visitados = 0;

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

struct Comuna {
    int idx;
    float ratio;
    Comuna(int i, float r) : idx(i), ratio(r) {}
};

float calcular_ratio(int idx) {
    int cobertura_total = accumulate(cobertura[idx].begin(), cobertura[idx].end(), 0);
    return (cobertura_total == 0) ? INFINITY : static_cast<float>(costo[idx]) / cobertura_total;
}

vector<int> ordenar_comunas_heuristica() {
    vector<Comuna> comunas;
    for (int i = 0; i < N; ++i) comunas.emplace_back(i, calcular_ratio(i));
    sort(comunas.begin(), comunas.end(), [](const Comuna& a, const Comuna& b) {
        return a.ratio < b.ratio;
    });
    vector<int> orden;
    for (const auto& c : comunas) orden.push_back(c.idx);
    return orden;
}

// Backtracking con heurística y poda de costo
void backtracking_heuristic(int idx, const vector<int>& orden, int costo_actual = 0) {
    nodos_visitados++;

    // Poda: Si el costo acumulado ya supera el mejor, no explorar más
    if (costo_actual >= mejor_costo) return;

    if (idx == N) {
        bool todas_cubiertas = true;
        for (int i = 0; i < N; ++i) {
            if (!es_cubierta(i)) {
                todas_cubiertas = false;
                break;
            }
        }
        if (todas_cubiertas) {
            if (costo_actual < mejor_costo) {
                mejor_costo = costo_actual;
                soluciones.clear();
                soluciones.push_back(x);
            } else if (costo_actual == mejor_costo) {
                soluciones.push_back(x);
            }
        }
        return;
    }

    int comuna_actual = orden[idx];

    // Probar construir (1)
    x[comuna_actual] = 1;
    backtracking_heuristic(idx + 1, orden, costo_actual + costo[comuna_actual]);

    // Probar no construir (0)
    x[comuna_actual] = 0;
    backtracking_heuristic(idx + 1, orden, costo_actual);
}

int main() {
    vector<int> orden = ordenar_comunas_heuristica();
    auto inicio = high_resolution_clock::now();
    backtracking_heuristic(0, orden, 0);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

        // Mostrar resultados
        if (!soluciones.empty()) {
            cout << "Se encontraron soluciones!" << endl;
            cout << "Costo total de la mejor solucion: " << mejor_costo << endl;
            cout << "Tiempo de ejecucion: " << duracion.count() << " milisegundos" << endl;
            cout << "Nodos visitados en el arbol: " << nodos_visitados << endl;
            cout << "Las soluciones son:" << endl;
            for (const auto& sol : soluciones) {
                cout << "Comunas con centro: ";
                for (int i = 0; i < N; ++i) {
                    if (sol[i] == 1) cout << i+1 << " ";
                }
                cout << endl;
            }
        } else {
            cout << "No se encontro solucion." << endl;
        }
    return 0;
}