import pandas as pd
import matplotlib.pyplot as plt
import os  # Para manejo de directorios

# Ruta de la carpeta 'output'
output_folder = "../output"

# Crear la carpeta 'output' si no existe
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Leer archivos
df_completa = pd.read_csv(os.path.join(output_folder, "completa.csv"))
df_heuristica = pd.read_csv(os.path.join(output_folder, "heuristica.csv"))

# Obtener últimos tiempos
ultimo_tiempo_heu = df_heuristica["Tiempo(ms)"].max()
ultimo_tiempo_comp = df_completa["Tiempo(ms)"].max()

# Crear figura
plt.figure(figsize=(12, 6))

# Graficar ambos algoritmos
plt.plot(df_completa["Tiempo(ms)"], df_completa["Costo"], 
         label='Algoritmo Completo', marker='o', markersize=5, linestyle='-', color='blue', linewidth=1.5)

plt.plot(df_heuristica["Tiempo(ms)"], df_heuristica["Costo"], 
         label='Heurística', marker='s', markersize=5, linestyle='-', color='red', linewidth=1.5)

# Añadir líneas verticales y textos
plt.axvline(ultimo_tiempo_heu, color='red', linestyle='--', alpha=0.7, linewidth=1)
plt.text(ultimo_tiempo_heu + 0.1, 200, f'Fin heurística: {ultimo_tiempo_heu:.2f} ms', color='red', fontsize=10)

plt.axvline(ultimo_tiempo_comp, color='blue', linestyle='--', alpha=0.7, linewidth=1)
plt.text(ultimo_tiempo_comp + 0.1, 300, f'Fin completo: {ultimo_tiempo_comp:.2f} ms', color='blue', fontsize=10)

# Detalles estéticos
plt.title('Evolución del Costo - Comparación Completa', fontsize=14)
plt.xlabel('Tiempo (milisegundos)', fontsize=12)
plt.ylabel('Costo', fontsize=12)
plt.legend()
plt.grid(True, alpha=0.3)
plt.ylim(150, 1000)  # Ajustar para mejor visualización

# Guardar y mostrar
plt.tight_layout()
plt.savefig(os.path.join(output_folder, 'costvstime.png'), dpi=300)
plt.show()