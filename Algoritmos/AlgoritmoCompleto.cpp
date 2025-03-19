#include <iostream>
#include <vector>
#include <chrono>
#include <climits>
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
ofstream salida("completa.csv");

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

void backtracking(int idx) {
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
            auto ahora = steady_clock::now();
            double tiempo = duration<double>(ahora - inicio_tiempo).count() * 1000; // Milisegundos con decimales
            if (costo_actual < mejor_costo) {
                mejor_costo = costo_actual;
                salida << tiempo << "," << mejor_costo << endl;
            }
        }
        return;
    }

    x[idx] = 1;
    backtracking(idx + 1);

    x[idx] = 0;
    backtracking(idx + 1);
}

int main() {
    inicio_tiempo = steady_clock::now();
    salida << "Tiempo(ms),Costo" << endl;
    backtracking(0);
    salida.close();
    return 0;
}