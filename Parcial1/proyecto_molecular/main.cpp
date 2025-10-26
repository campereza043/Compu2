#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sys/stat.h>
#include "Sistema.h"

/**
 * @brief Crea un directorio si no existe
 * @param path Ruta del directorio a crear
 */
void crearDirectorio(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        // Directorio no existe, crearlo
        #if defined(_WIN32)
            mkdir(path.c_str());
        #else
            mkdir(path.c_str(), 0733);
        #endif
    }
}

int main() {
    std::cout << "============================================\n";
    std::cout << "    SIMULACIÓN DE N PARTÍCULAS EN CAJA\n";
    std::cout << "============================================\n\n";
    
    // Crear directorios necesarios
    crearDirectorio("results");
    crearDirectorio("scripts");
    
    // PARÁMETROS CONFIGURABLES
    int N;
    double W, H;
    double m, r;
    double v0;
    double t_total, dt;
    int guardar_cada;
    
    // ENTRADA DE USUARIO
    std::cout << "Número de partículas (N): ";
    std::cin >> N;
    
    std::cout << "Dimensiones de la caja (W H): ";
    std::cin >> W >> H;
    
    std::cout << "Masa y radio de las partículas (m r): ";
    std::cin >> m >> r;
    
    std::cout << "Velocidad inicial máxima: ";
    std::cin >> v0;
    
    std::cout << "Tiempo total de simulación y paso temporal (t_total dt): ";
    std::cin >> t_total >> dt;
    
    std::cout << "Guardar datos cada n pasos (entero, ej: 10): ";
    std::cin >> guardar_cada;
    
    // VALIDACIONES
    if (N <= 0) {
        std::cerr << "Error: N debe ser positivo" << std::endl;
        return 1;
    }
    
    if (guardar_cada <= 0) {
        std::cerr << "Error: guardar_cada debe ser positivo" << std::endl;
        return 1;
    }
    
    if (r >= 0.1 * std::min(W, H)) {
        std::cerr << "Advertencia: Radio muy grande para la caja" << std::endl;
        std::cout << "¿Continuar? (s/n): ";
        char respuesta;
        std::cin >> respuesta;
        if (respuesta != 's' && respuesta != 'S') {
            return 1;
        }
    }
    
    // CONFIGURACIÓN DEL SISTEMA
    Sistema sistema;
    try {
        sistema.DefinaCaja(W, H);
        sistema.Reserve(N);
        
        // ELECCIÓN DE INICIALIZACIÓN
        char opcion;
        std::cout << "\nTipo de inicialización:\n";
        std::cout << "  [R] Rejilla ordenada\n";
        std::cout << "  [A] Aleatoria\n";
        std::cout << "Seleccione (R/A): ";
        std::cin >> opcion;
        
        if (opcion == 'R' || opcion == 'r') {
            bool alterna;
            std::cout << "¿Velocidades alternadas? (1=si, 0=no): ";
            std::cin >> alterna;
            sistema.InicialiceRejilla(m, r, v0, alterna);
            std::cout << "Sistema inicializado en rejilla" << std::endl;
        } else {
            // Inicialización aleatoria por defecto
            if (!sistema.InicialiceAleatorio(m, r, v0, 10000)) {
                std::cerr << "Error: No se pudo inicializar aleatoriamente después de 10000 intentos" << std::endl;
                std::cerr << "Intente con menos partículas o radio más pequeño" << std::endl;
                return 1;
            }
            std::cout << "Sistema inicializado aleatoriamente" << std::endl;
        }
        
        // ARCHIVO DE SALIDA
        std::ofstream fout("results/datos_simulacion.txt");
        if (!fout) {
            std::cerr << "Error: No se pudo crear el archivo de salida" << std::endl;
            return 1;
        }
        
        fout << std::scientific << std::setprecision(6);
        sistema.Encabezado(fout);
        
        // BUCLE PRINCIPAL DE SIMULACIÓN
        std::cout << "\nIniciando simulación..." << std::endl;
        std::cout << "Tiempo total: " << t_total << ", Paso: " << dt << std::endl;
        std::cout << "Partículas: " << N << std::endl;
        std::cout << "Guardando cada " << guardar_cada << " pasos" << std::endl;
        
        int n_pasos = static_cast<int>(t_total / dt);
        std::cout << "Total de pasos: " << n_pasos << std::endl;
        
        for (int paso = 0; paso <= n_pasos; ++paso) {
            double t = paso * dt;
            
            // Guardar datos periódicamente
            if (paso % guardar_cada == 0) {
                sistema.Guarde(fout, t);
                
                // Mostrar progreso cada 10%
                if (n_pasos > 0 && paso % (n_pasos / 10 + 1) == 0) {
                    std::cout << "Progreso: " << (100.0 * paso / n_pasos) << "%" << std::endl;
                }
            }
            
            // Avanzar simulación
            sistema.Paso(dt);
        }
        
        fout.close();
       // ARCHIVO DE SALIDA
std::ofstream fout("results/datos_simulacion.txt");
if (!fout) {
    std::cerr << "Error: No se pudo crear el archivo de salida" << std::endl;
    std::cerr << "Asegúrese de que la carpeta 'results' existe" << std::endl;
    return 1;
}

// Configurar formato de salida
fout << std::scientific << std::setprecision(6);
sistema.Encabezado(fout);
fout.flush(); // Forzar escritura del encabezado

std::cout << "\nIniciando simulación..." << std::endl;
std::cout << "Tiempo total: " << t_total << ", Paso: " << dt << std::endl;
std::cout << "Partículas: " << N << std::endl;
std::cout << "Guardando cada " << guardar_cada << " pasos" << std::endl;

int n_pasos = static_cast<int>(t_total / dt);
std::cout << "Total de pasos: " << n_pasos << std::endl;

for (int paso = 0; paso <= n_pasos; ++paso) {
    double t = paso * dt;
    
    // Guardar datos periódicamente
    if (paso % guardar_cada == 0) {
        sistema.Guarde(fout, t);
        fout.flush(); // Forzar escritura después de cada guardado
        
        // Mostrar progreso cada 10%
        if (n_pasos > 10 && paso % (n_pasos / 10) == 0) {
            std::cout << "Progreso: " << (100.0 * paso / n_pasos) << "%" << std::endl;
        }
    }
    
    // Avanzar simulación
    sistema.Paso(dt);
}

fout.close(); std::cout << "Simulación completada!" << std::endl;
        std::cout << "Datos guardados en: results/datos_simulacion.txt" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error durante la simulación: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
