/**
 * @file main.cpp
 * @brief Punto de entrada de la simulación del Flashing Ratchet.
 * * Modos de ejecución:
 * 1. Interactivo: ./simulacion (solicita parámetros)
 * 2. Batch: ./simulacion <T_on> <T_off> <total_time> <gamma> <kBT>
 * * @note Al finalizar, ejecuta automáticamente el script de graficación.
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib> // Necesario para std::system
#include "../include/MotorModel.h"
#include "../include/Integrator.h"

void showUsage(const char* progName) {
    std::cout << "Uso: " << progName << " [T_on T_off total_time gamma kBT]\n"
              << "O ejecute sin argumentos para modo interactivo.\n";
}

int main(int argc, char* argv[]) {
    // --- 1. CONFIGURACIÓN Y PARÁMETROS ---
    
    // Valores fijos para este examen/modelo
    double m = 1.0;          
    double dt = 0.005;       
    
    // Variables de usuario
    double T_on, T_off, total_time, gamma, kBT;

    if (argc >= 6) {
        // MODO BATCH
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

    // --- 2. INICIALIZACIÓN DEL SISTEMA ---
    
    MotorModel motor(m, gamma, kBT, T_on, T_off);
    VelocityVerlet integrator;

    // Configuración de archivo de salida
    // Se asume que la carpeta 'results' ya fue creada por CMake
    std::string filename = "../results/datos_ratchet.dat";
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: No se pudo abrir " << filename << ". Asegurate de que la carpeta 'results' exista." << std::endl;
        return 1;
    }

    // Cabecera del archivo .dat
    outfile << "t\tx\tv\tstate\tE_total\n";

    // Calcular fuerzas iniciales antes del primer paso
    motor.computeForces(dt);

    // --- 3. BUCLE DE SIMULACIÓN ---
    
    int print_freq = 20; // Guardar cada 20 pasos (aprox dt*20 = 0.1s)
    int step_count = 0;

    for (double t = 0; t < total_time; t += dt) {
        // Guardar datos
        if (step_count % print_freq == 0) { 
            outfile << t << "\t" 
                    << motor.p.x << "\t" 
                    << motor.p.v << "\t" 
                    << motor.chemistry.getState() << "\t"
                    << motor.getTotalEnergy() << "\n";
        }
        
        // Avanzar un paso temporal
        integrator.step(motor, dt);
        step_count++;
    }

    // Cerrar archivo para asegurar que todos los datos se escriban en disco
    outfile.close();
    std::cout << "Simulacion completada." << std::endl;
    std::cout << "Datos guardados en: " << filename << std::endl;

    // --- 4. GENERACIÓN AUTOMÁTICA DE GRÁFICAS ---
    
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Ejecutando script de graficacion..." << std::endl;

    // Llamada al sistema para ejecutar Python
    // NOTA: Se asume que el ejecutable está en 'build/' y el script en 'scripts/'
    // El comando busca el script subiendo un nivel y entrando a scripts.
    int plot_status = std::system("python3 ../scripts/plot_results.py");

    if (plot_status == 0) {
        std::cout << ">> Graficas generadas exitosamente en la carpeta 'results'." << std::endl;
    } else {
        std::cerr << ">> Advertencia: No se pudo ejecutar el script de Python." << std::endl;
        std::cerr << "   Verifique que 'plot_results.py' este en '../scripts/' y tenga Python instalado." << std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;

    return 0;
}