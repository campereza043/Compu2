#include <iostream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <stdexcept>
#include "Sistema.h"

int main() {
    Sistema sim;
    const double dt_sim = 0.001, dt_frame = 0.01;
    const double m = 1.0, r = 0.2, vmax = 1.0;
    double tf, W, H;
    int N;
    std::string integrador_nombre;

    // --- User Input ---
    std::cout << "Ingrese el numero de particulas (N): ";
    std::cin >> N;
    std::cout << "Ingrese el tiempo total de simulacion (s): ";
    std::cin >> tf;
    std::cout << "Ingrese el ancho de la caja:";
    std::cin >> W;
    std::cout << "Ingrese el alto de la caja: ";
    std::cin >> H;
    std::cout << "Elija el integrador (euler/verlet): ";
    std::cin >> integrador_nombre;

    // --- Setup Simulation ---
    try {
        sim.SeleccioneIntegrador(integrador_nombre);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    sim.DefinaCaja(W, H);
    sim.Reserve(N);
    sim.InicialiceRejilla(m, r, vmax);

    // --- Prepare Output File ---
std::filesystem::create_directories("../results");
std::ofstream archivo("../results/trayectorias.dat");
// ... (Verificación de apertura)

// **Añadir estas líneas para Gnuplot y Python**
archivo << "# W: " << W << "\n";
archivo << "# H: " << H << "\n";
archivo << "# R_BOLA: " << r << "\n";

// --- Simulation Loop ---
std::cout << "Iniciando simulacion con el integrador '" << integrador_nombre << "'..." << std::endl;
double t = 0;
long pasos_por_frame = static_cast<long>(dt_frame / dt_sim);

    sim.Encabezado(archivo); // Write header once

    while (t <= tf) {
        sim.Guarde(archivo, t); // Save current state

        // Advance simulation until the next frame time
        for(long i = 0; i < pasos_por_frame; ++i) {
            sim.Paso(dt_sim);
        }
        t += dt_frame;
        // Simple progress indicator
        std::cout << "\rProgreso: " << std::fixed << std::setprecision(1) << (t / tf) * 100.0 << "%" << std::flush;
    }
    std::cout << "\n Simulacion completada. Datos guardados en ../results/trayectorias.dat\n";
    archivo.close();

    // --- Plotting Choice ---
    std::cout << "Generar animacion con (p)ython o (g)nuplot? ";
    char op;
    std::cin >> op;

    if (op == 'p' || op == 'P') {
        std::cout << "Ejecutando script de Python..." << std::endl;
        system("python3 ../scripts/graficar.py");
    } else if (op == 'g' || op == 'G') {
        std::cout << "Ejecutando script de Gnuplot..." << std::endl;
        system("gnuplot ../scripts/graficar.gnuplot");
    }

    return 0;
}
