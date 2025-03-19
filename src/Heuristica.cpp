#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <numeric>
#include <cmath>
#include <fstream>
#include <sstream>
#include <filesystem> // Para manejo de directorios

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem; // Alias para simplificar

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
int nodos_visitados = 0; // Contador de nodos
steady_clock::time_point inicio_tiempo;
ofstream salida;

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
    nodos_visitados++; // Incrementar nodos visitados

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
            double tiempo = duration<double>(ahora - inicio_tiempo).count() * 1000;
            mejor_costo = costo_actual;
            soluciones.clear();
            soluciones.push_back(x);
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
    // Ruta de la carpeta '../output'
    string output_folder = "../output";

    // Crear la carpeta '../output' si no existe
    if (!fs::exists(output_folder)) {
        fs::create_directory(output_folder);
    }

    // Abrir el archivo CSV en '../output/heuristica.csv'
    salida.open(output_folder + "/heuristica.csv");
    if (!salida.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        return 1;
    }

    inicio_tiempo = steady_clock::now();
    vector<int> orden = ordenar_comunas_heuristica();
    salida << "Tiempo(ms),Costo" << endl;
    backtracking_heuristic(0, orden, 0);
    salida.close();

    // Leer último tiempo del CSV
    double ultimo_tiempo = 0.0;
    ifstream entrada(output_folder + "/heuristica.csv");
    string linea;
    while (getline(entrada, linea)) {
        if (linea.empty() || linea.find("Tiempo") != string::npos) continue;
        stringstream ss(linea);
        string tiempo_str;
        getline(ss, tiempo_str, ',');
        ultimo_tiempo = stod(tiempo_str);
    }

    // Mostrar resultados por consola
    cout << "\n=== RESULTADOS HEURÍSTICA ===" << endl;
    cout << "- Nodos visitados: " << nodos_visitados << endl;
    cout << "- Tiempo total: " << ultimo_tiempo << " ms" << endl;
    cout << "- Mejor costo: " << mejor_costo << endl;
    cout << "- Solucion(es):" << endl;
    for (const auto& sol : soluciones) {
        cout << "  Comunas: ";
        for (int i = 0; i < N; ++i) {
            if (sol[i] == 1) cout << i + 1 << " ";
        }
        cout << endl;
    }

    return 0;
}