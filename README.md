# Tarea_1_Meta
En esta tarea se desarrollan dos algoritmos para encontrar la solución a un problema de optimización.
---

## *Buildear Algoritmos* 🛠️
Para buildear los algoritmos respectivos debemos ejecutar los siguientes comandos:

´´´cpp
cd .\src\
´´´
Luego para buildear cada script asociada a cada técnica:

1. Para la técnica completa:
´´´cpp
g++ AlgoritmoCompleto.cpp -o AlgoritmoCompleto
´´´
2. Para la heurística:
´´´cpp
g++ Heuristica.cpp -o Heuristica.exe
´´´
---

## *Correr algoritmos* ⚙️
Una vez buildeado el script, ejecutar en orden:
´´´cpp
cd .\src\
´´´
Luego para cada algoritmo:

1. Para la técnica completa:
´´´cpp
.\AlgoritmoCompleto.exe
´´´
2. Para la heurística:
´´´cpp
.Heuristica.exe
´´´

## *Graficos* 📊
Para poder medir los costos en relación al tiempo, se genera un gráfico que se puede crear con ayuda del siguiente script:

´´´cpp
cd .\test\
python .graficos.py
´´´