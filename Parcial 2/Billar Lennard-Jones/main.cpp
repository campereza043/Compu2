/**
 * @file main.cpp
 * @brief Ejecución del Gas de Lennard-Jones.
 */

#include <iostream>
#include <filesystem>
#include "Sistema.h"

int main() {
    Sistema sim;
    
    // Parámetros de Simulación (Unidades reducidas o arbitrarias)
    const double dt = 0.001;     // Paso de tiempo pequeño para estabilidad LJ
    const double tf = 10.0;      // Tiempo final
    const double W = 20.0;       // Ancho
    const double H = 20.0;       // Alto
    const int N = 10;            // Número de partículas
    
    // Propiedades Físicas
    const double m = 1.0;
    const double r_viz = 0.5;    // Radio visual (para graficar)
    const double eps = 1.0;      // Profundidad del pozo de potencial
    const double sigma = 1.0;    // Distancia donde V=0 (aprox diámetro efectivo)
    const double vmax = 2.0;

    // Configuración
    sim.DefinaCaja(W, H);
    sim.DefinaParametrosLJ(eps, sigma);
    sim.Reserve(N);
    sim.InicialiceRejilla(m, r_viz, vmax);

    // Salida
    std::filesystem::create_directories("../results");
    std::ofstream archivo("../results/trayectorias.csv"); // Uso CSV para facilitar análisis

    sim.Encabezado(archivo);

    std::cout << "Iniciando simulacion de Gas Lennard-Jones..." << std::endl;
    std::cout << "Integrador: Velocity-Verlet" << std::endl;
    std::cout << "Particulas: " << N << std::endl;

    double t = 0;
    int frame = 0;
    int pasos_por_frame = 10; // Guardar cada 10 pasos

    while (t < tf) {
        if (frame % pasos_por_frame == 0) {
            sim.Guarde(archivo, t);
            std::cout << "\rTiempo: " << t << " / " << tf << std::flush;
        }
        
        sim.Paso(dt);
        t += dt;
        frame++;
    }

    std::cout << "\nSimulacion completada." << std::endl;
    archivo.close();

    return 0;
}