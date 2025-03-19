#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <numeric>   // Para accumulate
#include <cmath>     // Para INFINITY

using namespace std;
using namespace std::chrono;

const int N = 15;
vector<int> costo = {60, 30, 60, 70, 130, 60, 70, 60, 80, 70, 50, 90, 30, 30, 100};

// Matriz de cobertura 15x15 (filas: comunas, columnas: cobertura)
vector<vector<int>> cobertura = {
    {1,1,1,1,0,0,0,0,0,0,0,0,1,0,0},    // Comuna 1
    {1,1,0,1,0,0,0,0,0,0,0,1,0,0,1},    // Comuna 2
    {1,0,1,1,1,1,0,0,0,0,0,0,1,0,0},    // Comuna 3
    {1,1,1,1,1,0,0,0,0,0,0,1,0,0,0},    // Comuna 4
    {0,0,1,1,1,1,1,1,1,0,0,1,0,0,0},    // Comuna 5
    {0,0,1,0,1,1,0,0,1,0,0,0,0,0,0},    // Comuna 6
    {0,0,0,0,1,0,1,1,0,1,1,1,0,1,1},    // Comuna 7
    {0,0,0,0,1,0,1,1,1,1,0,0,0,0,0},    // Comuna 8
    {0,0,0,0,1,1,0,1,1,1,1,0,0,0,0},    // Comuna 9
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},    // Comuna 10
    {0,0,0,0,0,0,1,0,1,1,1,0,0,1,0},    // Comuna 11
    {0,1,0,1,1,0,1,0,0,0,0,1,0,0,1},    // Comuna 12
    {1,0,1,0,0,0,0,0,0,0,0,0,1,0,0},    // Comuna 13
    {0,0,0,0,0,0,1,0,0,0,1,0,0,1,1},    // Comuna 14
    {0,1,0,0,0,0,1,0,0,0,0,1,0,1,1}     // Comuna 15
};

vector<int> x(N, 0);
vector<vector<int>> soluciones;
int mejor_costo = INT_MAX;
int nodos_visitados = 0;

// Función para calcular el costo total
int costo_total() {
    int total = 0;
    for (int i = 0; i < N; ++i) {
        if (x[i] == 1) total += costo[i];
    }
    return total;
}

// Función para verificar si una comuna está cubierta
bool es_cubierta(int comuna) {
    for (int i = 0; i < N; ++i) {
        if (x[i] == 1 && cobertura[i][comuna] == 1) return true;
    }
    return false;
}

// Estructura para ordenar comunas por heurística
struct Comuna {
    int idx;
    float ratio;
    Comuna(int i, float r) : idx(i), ratio(r) {}
};

// Calcular ratio costo/cobertura
float calcular_ratio(int idx) {
    int cobertura_total = accumulate(cobertura[idx].begin(), cobertura[idx].end(), 0);
    return (cobertura_total == 0) ? INFINITY : static_cast<float>(costo[idx]) / cobertura_total;
}

// Ordenar comunas por ratio (ascendente)
vector<int> ordenar_comunas_heuristica() {
    vector<Comuna> comunas;
    for (int i = 0; i < N; ++i) {
        comunas.emplace_back(i, calcular_ratio(i));
    }
    sort(comunas.begin(), comunas.end(), [](const Comuna& a, const Comuna& b) {
        return a.ratio < b.ratio;
    });
    vector<int> orden;
    for (const auto& c : comunas) orden.push_back(c.idx);
    return orden;
}

// Backtracking con orden heurístico
void backtracking_heuristic(int idx, const vector<int>& orden) {
    nodos_visitados++;
    if (idx == N) {
        bool todas_cubiertas = true;
        for (int i = 0; i < N; ++i) {
            if (!es_cubierta(i)) {
                todas_cubiertas = false;
                break;
            }
        }
        if (todas_cubiertas) {
            int costo_actual = costo_total();
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

    int comuna_actual = orden[idx];  // Usar orden heurístico

    // Probar construir primero (1)
    x[comuna_actual] = 1;
    backtracking_heuristic(idx + 1, orden);

    // Probar no construir (0)
    x[comuna_actual] = 0;
    backtracking_heuristic(idx + 1, orden);
}

int main() {
    vector<int> orden = ordenar_comunas_heuristica();  // Orden heurístico

    auto inicio = high_resolution_clock::now();
    backtracking_heuristic(0, orden);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    if (!soluciones.empty()) {
        cout << "Se encontraron soluciones!" << endl;
        cout << "Costo total optimo: " << mejor_costo << endl;
        cout << "Tiempo de ejecucion: " << duracion.count() << " milisegundos" << endl;
        cout << "Nodos visitados: " << nodos_visitados << endl;
        cout << "Comunas seleccionadas: " << endl;
        for (const auto& sol : soluciones) {
            for (int i = 0; i < N; ++i) {
                if (sol[i] == 1) cout << i + 1 << " ";
            }
            cout << endl;
        }
    } else {
        cout << "No hay solucion." << endl;
    }

    return 0;
}