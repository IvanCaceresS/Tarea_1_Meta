#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <numeric>
#include <cmath>
#include <fstream>

using namespace std;
using namespace std::chrono;

const int N = 15;
vector<int> costo = {60, 30, 60, 70, 130, 60, 70, 60, 80, 70, 50, 90, 30, 30, 100};
vector<vector<int>> cobertura = {
    {1,1,1,1,0,0,0,0,0,0,0,0,1,0,0},
    {1,1,0,1,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,0,0,0,0,0,0,1,0,0},
    {1,1,1,1,1,0,0,0,0,0,0,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0,1,0,0,0},
    {0,0,1,0,1,1,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,1,0,1,1,0,1,1,1,0,1,1},
    {0,0,0,0,1,0,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,1,1,0,1,1,1,1,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
    {0,0,0,0,0,0,1,0,1,1,1,0,0,1,0},
    {0,1,0,1,1,0,1,0,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,1,0,0,0,1,0,0,1,1},
    {0,1,0,0,0,0,1,0,0,0,0,1,0,1,1}
};

vector<int> x(N, 0);
vector<vector<int>> soluciones;
int mejor_costo = INT_MAX;
steady_clock::time_point inicio_tiempo;
ofstream salida("heuristica.csv");

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
    for (int i = 0; i < N; ++i) 
        comunas.emplace_back(i, calcular_ratio(i));
    sort(comunas.begin(), comunas.end(), [](const Comuna& a, const Comuna& b) {
        return a.ratio < b.ratio;
    });
    vector<int> orden;
    for (const auto& c : comunas) orden.push_back(c.idx);
    return orden;
}

int costo_total() {
    int total = 0;
    for (int i = 0; i < N; ++i) 
        if (x[i] == 1) total += costo[i];
    return total;
}

bool es_cubierta(int comuna) {
    for (int i = 0; i < N; ++i) 
        if (x[i] == 1 && cobertura[i][comuna] == 1) return true;
    return false;
}

void backtracking_heuristic(int idx, const vector<int>& orden, int costo_actual = 0) {
    if (costo_actual >= mejor_costo) return;

    if (idx == N) {
        bool todas_cubiertas = true;
        for (int i = 0; i < N; ++i) {
            if (!es_cubierta(i)) {
                todas_cubiertas = false;
                break;
            }
        }
        if (todas_cubiertas && costo_actual < mejor_costo) {
            auto ahora = steady_clock::now();
            double tiempo = duration<double>(ahora - inicio_tiempo).count() * 1000; // Milisegundos con decimales
            mejor_costo = costo_actual;
            salida << tiempo << "," << mejor_costo << endl;
        }
        return;
    }

    int comuna_actual = orden[idx];
    x[comuna_actual] = 1;
    backtracking_heuristic(idx + 1, orden, costo_actual + costo[comuna_actual]);

    x[comuna_actual] = 0;
    backtracking_heuristic(idx + 1, orden, costo_actual);
}

int main() {
    inicio_tiempo = steady_clock::now();
    vector<int> orden = ordenar_comunas_heuristica();
    salida << "Tiempo(ms),Costo" << endl;
    backtracking_heuristic(0, orden, 0);
    salida.close();
    return 0;
}