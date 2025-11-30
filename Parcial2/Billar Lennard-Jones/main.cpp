/**
 * @file main.cpp
 * @brief Ejecución del Gas de Lennard-Jones con inputs de usuario y llamada a Python.
 */

#include <iostream>
#include <filesystem>
#include <string>
#include "Sistema.h"

int main() {
    Sistema sim;
    
    // Variables para input del usuario
    double tf, W, H;
    int N;

    // --- 1. Solicitud de Datos al Usuario ---
    std::cout << "=== Configuracion Simulación Gas LJ ===" << std::endl;
    
    std::cout << "Ingrese el numero de particulas (N) [ej. 20]: ";
    std::cin >> N;
    
    std::cout << "Ingrese el tiempo de simulacion (tf) [ej. 10.0]: ";
    std::cin >> tf;
    
    std::cout << "Ingrese el ancho de la caja (W) [ej. 20.0]: ";
    std::cin >> W;
    
    std::cout << "Ingrese el alto de la caja (H) [ej. 20.0]: ";
    std::cin >> H;

    // Parámetros Físicos Fijos (o podrían pedirse también)
    const double dt = 0.001;     
    const double m = 1.0;
    const double r_viz = 0.2;    
    const double eps = 1.0;      
    const double sigma = 1.8;    
    const double vmax = 4.0;

    // --- 2. Configuración del Sistema ---
    sim.DefinaCaja(W, H);
    sim.DefinaParametrosLJ(eps, sigma);
    sim.Reserve(N);
    sim.InicialiceRejilla(m, r_viz, vmax);

    // --- 3. Preparación de Archivo .dat ---
    std::filesystem::create_directories("../results");
    // Cambio de extensión a .dat
    std::string filename = "../results/trayectorias.dat";
    std::ofstream archivo(filename); 

    if (!archivo.is_open()) {
        std::cerr << "Error al crear el archivo de salida." << std::endl;
        return 1;
    }

    sim.Encabezado(archivo);

    std::cout << "\nIniciando simulacion..." << std::endl;
    std::cout << "Guardando datos en: " << filename << std::endl;

    double t = 0;
    int frame = 0;
    int pasos_por_frame = 100; 

    // --- 4. Bucle Principal ---
    while (t < tf) {
        if (frame % pasos_por_frame == 0) {
            sim.Guarde(archivo, t);
            // Barra de progreso simple
            int progress = (int)((t / tf) * 100.0);
            std::cout << "\rProgreso: " << progress << "%" << std::flush;
        }
        
        sim.Paso(dt);
        t += dt;
        frame++;
    }

    std::cout << "\rProgreso: 100%" << std::endl;
    std::cout << "Simulacion completada." << std::endl;
    archivo.close();

    // --- 5. Ejecución Automática de Python ---
    // Se asume que graficar.py está en el directorio superior (raíz del proyecto)
    // Se pasan W y H como argumentos para que Python ajuste los ejes.
    std::cout << "Ejecutando script de visualizacion..." << std::endl;
    
    std::string command = "python3 ../scripts/graficar.py " + std::to_string(W) + " " + std::to_string(H);
    
    // Si estás en Windows y python3 no funciona, intenta con "python"
    // std::string command = "python ../graficar.py " + std::to_string(W) + " " + std::to_string(H);

    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Hubo un error al ejecutar el script de Python." << std::endl;
        std::cerr << "Asegurate de tener instalados: pandas, numpy, matplotlib" << std::endl;
    }

    return 0;
}