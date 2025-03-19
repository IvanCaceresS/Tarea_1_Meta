import pandas as pd
import matplotlib.pyplot as plt

# Leer archivos individuales
df_completa = pd.read_csv("../Algoritmos/completa.csv")
df_heuristica = pd.read_csv("../Algoritmos/heuristica.csv")

# Unir datos usando el tiempo como índice
df_completa = df_completa.rename(columns={"Costo": "Completa"})
df_heuristica = df_heuristica.rename(columns={"Costo": "Heuristica"})

df_final = pd.merge(df_completa, df_heuristica, on="Tiempo(ms)", how="outer").sort_values("Tiempo(ms)")
df_final = df_final.ffill().fillna(method='bfill')  # Interpolar hacia adelante y atrás

# Gráfico
plt.figure(figsize=(12, 6))
plt.plot(df_final["Tiempo(ms)"], df_final["Completa"], label='Completa', marker='.', color='blue')
plt.plot(df_final["Tiempo(ms)"], df_final["Heuristica"], label='Heurística', marker='.', linestyle='--', color='red')

# Añadir anotaciones del costo mínimo
min_completa = df_final["Completa"].min()
min_heuristica = df_final["Heuristica"].min()
plt.axhline(min_completa, color='blue', linestyle=':', alpha=0.5)
plt.axhline(min_heuristica, color='red', linestyle=':', alpha=0.5)
plt.text(9.5, min_completa + 5, f'Mínimo Completa: {min_completa}', color='blue')
plt.text(9.5, min_heuristica - 15, f'Mínimo Heurística: {min_heuristica}', color='red')

# Detalles estéticos
plt.title('Evolución del Costo de Soluciones', fontsize=14)
plt.xlabel('Tiempo (milisegundos)', fontsize=12)
plt.ylabel('Costo', fontsize=12)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

# Guardar y mostrar
plt.savefig('comparacion.png', dpi=300)
plt.show()