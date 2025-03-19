#include <iostream>
#include <vector>
#include <chrono>
#include <climits>
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
    nodos_visitados++; // Incrementar nodos visitados

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
            double tiempo = duration<double>(ahora - inicio_tiempo).count() * 1000;
            if (costo_actual < mejor_costo) {
                mejor_costo = costo_actual;
                soluciones.clear();
                soluciones.push_back(x);
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
    // Ruta de la carpeta '../output'
    string output_folder = "../output";

    // Crear la carpeta '../output' si no existe
    if (!fs::exists(output_folder)) {
        fs::create_directory(output_folder);
    }

    // Abrir el archivo CSV en '../output/completa.csv'
    salida.open(output_folder + "/completa.csv");
    if (!salida.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        return 1;
    }

    inicio_tiempo = steady_clock::now();
    salida << "Tiempo(ms),Costo" << endl;
    backtracking(0);
    salida.close();

    // Leer último tiempo del CSV
    double ultimo_tiempo = 0.0;
    ifstream entrada(output_folder + "/completa.csv");
    string linea;
    while (getline(entrada, linea)) {
        if (linea.empty() || linea.find("Tiempo") != string::npos) continue;
        stringstream ss(linea);
        string tiempo_str;
        getline(ss, tiempo_str, ',');
        ultimo_tiempo = stod(tiempo_str);
    }

    // Mostrar resultados por consola
    cout << "\n=== RESULTADOS ALGORITMO COMPLETO ===" << endl;
    cout << "- Nodos visitados: " << nodos_visitados << endl;
    cout << "- Tiempo total: " << ultimo_tiempo << " ms" << endl;
    cout << "- Mejor costo: " << mejor_costo << endl;
    cout << "- Solucion(es):" << endl;
    for (const auto& sol : soluciones) {
        cout << "  Comunias: ";
        for (int i = 0; i < N; ++i) {
            if (sol[i] == 1) cout << i + 1 << " ";
        }
        cout << endl;
    }

    return 0;
}