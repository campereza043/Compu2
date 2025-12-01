/**
 * @file main.cpp
 * @brief Punto de entrada de la simulación del Flashing Ratchet.
 * * Permite dos modos de ejecución:
 * 1. Interactivo: Si se ejecuta sin argumentos, solicita parámetros al usuario.
 * 2. Batch: ./simulacion <T_on> <T_off> <total_time> <gamma> <kBT>
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include "../include/MotorModel.h"
#include "../include/Integrator.h"

void showUsage(const char* progName) {
    std::cout << "Uso: " << progName << " [T_on T_off total_time gamma kBT]\n"
              << "O ejecute sin argumentos para modo interactivo.\n";
}

int main(int argc, char* argv[]) {
    // Valores por defecto / Variables
    double m = 1.0;          // Masa (fija para este examen)
    double dt = 0.005;       // Paso de tiempo (fijo para estabilidad)
    
    // Parámetros modificables por el usuario
    double T_on, T_off, total_time, gamma, kBT;

    // --- LOGICA DE ENTRADA DE USUARIO ---
    if (argc >= 6) {
        // MODO BATCH (Argumentos de linea de comandos)
        // Orden: T_on T_off total_time gamma kBT
        try {
            T_on = std::atof(argv[1]);
            T_off = std::atof(argv[2]);
            total_time = std::atof(argv[3]);
            gamma = std::atof(argv[4]);
            kBT = std::atof(argv[5]);
            std::cout << "Modo Batch activado. Parametros cargados." << std::endl;
        } catch (...) {
            std::cerr << "Error al leer argumentos." << std::endl;
            showUsage(argv[0]);
            return 1;
        }
    } else {
        // MODO INTERACTIVO
        std::cout << "=== Configuracion Flashing Ratchet ===" << std::endl;
        
        std::cout << "Ingrese Tiempo Encendido (T_on) [Sugerido 50.0]: ";
        std::cin >> T_on;
        
        std::cout << "Ingrese Tiempo Apagado (T_off) [Sugerido 20.0]: ";
        std::cin >> T_off;
        
        std::cout << "Ingrese Tiempo Total Simulacion [Sugerido 1000.0]: ";
        std::cin >> total_time;
        
        std::cout << "Ingrese Coeficiente Friccion (gamma) [Sugerido 0.5]: ";
        std::cin >> gamma;
        
        std::cout << "Ingrese Temperatura (kBT) [Sugerido 0.8]: ";
        std::cin >> kBT;
        
        std::cout << "--- Iniciando simulacion... ---" << std::endl;
    }

    // Instanciar Modelo e Integrador
    MotorModel motor(m, gamma, kBT, T_on, T_off);
    VelocityVerlet integrator;

    // Configuración de archivo de salida
    std::string filename = "../results/datos_ratchet.dat";
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: No se pudo abrir " << filename << ". Asegurate de que la carpeta 'results' exista." << std::endl;
        return 1;
    }

    // Cabecera del archivo
    outfile << "t\tx\tv\tstate\tE_total\n";

    // Calcular fuerzas iniciales
    motor.computeForces(dt);

    // Bucle de simulación
    int print_freq = 20; // Guardar cada 20 pasos (aprox dt*20 = 0.1s)
    int step_count = 0;

    for (double t = 0; t < total_time; t += dt) {
        if (step_count % print_freq == 0) { 
            outfile << t << "\t" 
                    << motor.p.x << "\t" 
                    << motor.p.v << "\t" 
                    << motor.chemistry.getState() << "\t"
                    << motor.getTotalEnergy() << "\n";
        }
        integrator.step(motor, dt);
        step_count++;
    }

    outfile.close();
    std::cout << "Simulacion completada exitosamente." << std::endl;
    std::cout << "Datos guardados en: " << filename << std::endl;

    return 0;
}