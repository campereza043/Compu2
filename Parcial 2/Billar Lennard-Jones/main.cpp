/**
 * @file main.cpp
 * @brief Programa principal que ejecuta la simulación del sistema de bolas en una caja.
 * 
 * Solicita los parámetros al usuario, configura el sistema y guarda
 * los resultados en un archivo para posterior visualización.
 */

#include <iostream>
#include <iomanip>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <cmath>
#include "Sistema.h"

/**
 * @brief Calcula la capacidad máxima de bolas en la caja
 * @param W Ancho de la caja
 * @param H Alto de la caja
 * @param r Radio de cada bola
 * @return Número máximo de bolas que caben en la caja
 */
int CalcularCapacidadMaxima(double W, double H, double r) {
    // Calcula cuántas bolas caben en una disposición de rejilla hexagonal
    // que es más eficiente que la cuadrada
    double diametro = 2 * r;
    int columnas = std::max(1, static_cast<int>((W - r) / diametro));
    int filas = std::max(1, static_cast<int>((H - r) / (diametro * std::sqrt(3) / 2)));
    
    return columnas * filas;
}

/**
 * @brief Función principal.
 * 
 * Configura los parámetros de la simulación, selecciona el integrador,
 * ejecuta la evolución temporal y guarda los datos en un archivo.
 * 
 * @return 0 si la simulación termina correctamente.
 */
int main() {
    Sistema sim;
    const double dt_sim = 0.001; ///< Paso interno de integración.
    const double dt_frame = 0.01; ///< Intervalo entre registros de salida.
    const double m = 1.0; ///< Masa de cada bola.
    const double r = 0.2; ///< Radio de cada bola.
    const double vmax = 4.0; ///< Velocidad máxima inicial.
    double tf, W, H;
    int N;
    std::string integrador_nombre;

    // --- Entrada de usuario ---
    std::cout << "Ingrese el numero de particulas (N): ";
    std::cin >> N;
    std::cout << "Ingrese el tiempo total de simulacion (s): ";
    std::cin >> tf;
    std::cout << "Ingrese el ancho de la caja: ";
    std::cin >> W;
    std::cout << "Ingrese el alto de la caja: ";
    std::cin >> H;
    
    // --- Validación de capacidad ---
    int capacidad_maxima = CalcularCapacidadMaxima(W, H, r);
    if (N > capacidad_maxima) {
        std::cerr << "\n  ADVERTENCIA: Demasiadas bolas para la caja!" << std::endl;
        std::cerr << "La caja de " << W << "x" << H << " con bolas de radio " << r 
                  << " puede contener como máximo " << capacidad_maxima << " bolas." << std::endl;
        std::cerr << "Has solicitado " << N << " bolas." << std::endl;
        std::cerr << "¿Deseas continuar de todos modos? (s/n): ";
        
        char respuesta;
        std::cin >> respuesta;
        if (respuesta != 's' && respuesta != 'S') {
            std::cout << "Simulación cancelada. Reduce el número de bolas o aumenta el tamaño de la caja." << std::endl;
            return 1;
        }
        std::cout << "Continuando con configuración sobrepoblada..." << std::endl;
    }
    
    std::cout << "Elija el integrador (euler/verlet): ";
    std::cin >> integrador_nombre;

    // --- Configuración del sistema ---
    try {
        sim.SeleccioneIntegrador(integrador_nombre);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    sim.DefinaCaja(W, H);
    sim.Reserve(N);
    sim.InicialiceRejilla(m, r, vmax);

    // --- Archivo de salida ---
    std::filesystem::create_directories("../results");
    std::ofstream archivo("../results/trayectorias.dat");

    archivo << "# W: " << W << "\n";
    archivo << "# H: " << H << "\n";
    archivo << "# R_BOLA: " << r << "\n";
    archivo << "# N_BOLAS: " << N << "\n";
    archivo << "# CAPACIDAD_MAXIMA_RECOMENDADA: " << capacidad_maxima << "\n";

    std::cout << "Iniciando simulacion con el integrador '" 
              << integrador_nombre << "'..." << std::endl;
    std::cout << "Configuración: " << N << " bolas en caja " << W << "x" << H 
              << " (capacidad recomendada: " << capacidad_maxima << ")" << std::endl;

    // --- Bucle principal de simulación ---
    double t = 0;
    long pasos_por_frame = static_cast<long>(dt_frame / dt_sim);

    sim.Encabezado(archivo);

    while (t <= tf) {
        sim.Guarde(archivo, t);
        for (long i = 0; i < pasos_por_frame; ++i)
            sim.Paso(dt_sim);
        t += dt_frame;

        std::cout << "\rProgreso: " 
                  << std::fixed << std::setprecision(1)
                  << (t / tf) * 100.0 << "%" << std::flush;
    }

    std::cout << "\nSimulacion completada. Datos guardados en ../results/trayectorias.dat\n";
    archivo.close();

    // --- Opción de visualización ---
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
