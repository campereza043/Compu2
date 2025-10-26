#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Necesario para system()
#include "Sistema.h"

int main() {
  // --- Parámetros de la simulación ---
  int N; // Número de partículas (lo ingresa el usuario)
  const double W = 100.0;     // Ancho de la caja
  const double H = 100.0;     // Alto de la caja
  const double M_BOLA = 1.0;  // Masa de las bolas
  const double R_BOLA = 2.0;  // Radio de las bolas
  const double V_MAX = 10.0;  // Velocidad inicial máxima

  const double T_TOTAL = 50.0;        // Tiempo total de simulación
  const double DT_SIM = 0.001;        // Paso de tiempo para la simulación (más preciso)
  const double DT_FRAME = 0.01;       // Paso de tiempo para guardar datos (para el GIF/video)

  // --- Solicitar número de partículas al usuario ---
  std::cout << "Ingrese el numero de particulas (pepas): ";
  std::cin >> N;

  if (N <= 0) {
    std::cerr << "El numero de particulas debe ser positivo." << std::endl;
    return 1;
  }

  // --- Configuración del sistema ---
  Sistema simulador;
  simulador.DefinaCaja(W, H);
  simulador.Reserve(N);
  simulador.InicialiceRejilla(M_BOLA, R_BOLA, V_MAX);

  // --- Archivo de salida ---
  std::ofstream outfile("results/datos.txt");
  if (!outfile.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo de salida." << std::endl;
    return 1;
  }
  simulador.Encabezado(outfile);

  // --- Bucle de simulación ---
  int pasos_por_frame = static_cast<int>(DT_FRAME / DT_SIM);
  double t = 0;
  
  std::cout << "Iniciando simulacion..." << std::endl;
  
  while (t <= T_TOTAL) {
    simulador.Guarde(outfile, t); // Guarda el estado actual
    
    // Avanza la simulación por varios pasos pequeños antes del siguiente fotograma
    for(int i = 0; i < pasos_por_frame; ++i) {
      simulador.Paso(DT_SIM);
    }
    t += DT_FRAME;
  }

  outfile.close();
  std::cout << "Simulacion completada." << std::endl;
  std::cout << "Datos guardados en 'results/datos.txt'" << std::endl;

  // --- Selección de Graficación ---
  char choice;
  std::cout << "\n¿Desea graficar los resultados?" << std::endl;
  std::cout << "  [p] Python (genera video .mp4)" << std::endl;
  std::cout << "  [g] Gnuplot (genera .gif)" << std::endl;
  std::cout << "  [n] No, salir" << std::endl;
  std::cout << "Elija una opcion: ";
  std::cin >> choice;

  if (choice == 'p' || choice == 'P') {
    std::cout << "Generando animacion con Python... (esto puede tardar un momento)" << std::endl;
    // Ejecutamos el script de python. Asumimos que python3 está en el PATH.
    system("python3 scripts/python/plot.py");
  } else if (choice == 'g' || choice == 'G') {
    std::cout << "Generando animacion con Gnuplot... (esto puede tardar un momento)" << std::endl;
    // Nos movemos al directorio del script para que encuentre el archivo de datos y luego volvemos.
    system("cd scripts/gnuplot && gnuplot plot.gp && cd ../..");
  } else {
    std::cout << "Saliendo del programa." << std::endl;
  }

  return 0;
}
