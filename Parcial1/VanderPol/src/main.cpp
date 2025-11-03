/**
 * @mainpage VanDerPol – Sistema de osciladores acoplados
 * @brief Manual técnico y de usuario
 * @details
 * Este proyecto simula y analiza un par de osciladores de Van der Pol
 * acoplados linealmente.  
 * Permite explorar fenómenos de **sincronización, caos y bifurcaciones**.
 *
 * ### 🔧 Compilación
 * ```bash
 * mkdir build && cd build
 * cmake ..
 * make
 * ```
 *
 * ### 🚀 Ejecución
 * ```bash
 * ./vanderpol --auto
 * ```
 *
 * ### 🧩 Opciones disponibles
 * - `--auto` Ejecuta todas las simulaciones y análisis.
 * - `--validate_dt` Prueba diferentes pasos de tiempo.
 * - `--poincare` Genera el mapa de Poincaré.
 * - `--lyapunov` Calcula el exponente de Lyapunov.
 * - `--gif` Genera animaciones de las trayectorias.
 * - `--interactive` Permite ingresar los parámetros manualmente.
 *
 * ### 📊 Resultados
 * Los resultados se almacenan en el directorio `results/`:
 * - `datos.txt`: simulación temporal.
 * - `poincare.txt`: puntos del mapa de Poincaré.
 * - `lyapunov_final.txt`: valor del exponente de Lyapunov.
 *
 * @author Camila Pérez, Sergio Carasquilla
 * @date 2025
 */


#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <limits>
#include "Sistema.h"

/*
 * Uso:
 * ./vanderpol [--auto] [--validate_dt] [--poincare] [--lyapunov] [--gif] plus params like before
 *
 * --auto : corre todo (simulate + plots + gif)
 * --validate_dt : realiza validación de dt
 * --poincare : genera mapa de Poincaré
 * --lyapunov : calcula exponentes de Lyapunov
 * --gif : genera GIF animado de Lissajous
 * --interactive : modo interactivo para ingresar parámetros
 */

double get_arg_or_default(int argc, char** argv, const std::string &key, double def) {
    for (int i = 1; i < argc-1; ++i) {
        if (std::string(argv[i]) == key) return std::atof(argv[i+1]);
    }
    return def;
}

std::string get_arg_or_default_str(int argc, char** argv, const std::string &key, const std::string &def) {
    for (int i = 1; i < argc-1; ++i) {
        if (std::string(argv[i]) == key) return std::string(argv[i+1]);
    }
    return def;
}

bool flag_present(int argc, char** argv, const std::string &flag) {
    for (int i=1;i<argc;++i) if (std::string(argv[i])==flag) return true;
    return false;
}

double ask_parameter(const std::string& prompt, double default_value) {
    double value;
    std::cout << prompt << " [" << default_value << "]: ";
    std::cin >> value;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Valor inválido. Usando valor por defecto: " << default_value << std::endl;
        return default_value;
    }
    return value;
}

void interactive_mode(double& mu1, double& w01, double& mu2, double& w02, double& k,
                     double& t0, double& tf, double& dt,
                     double& x10, double& v10, double& x20, double& v20) {
    std::cout << "\n=== MODO INTERACTIVO ===" << std::endl;
    std::cout << "Ingrese los parámetros de simulación:" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    std::cout << "\n--- PARÁMETROS DEL OSCILADOR 1 ---" << std::endl;
    mu1 = ask_parameter("Parámetro de no linealidad μ1", mu1);
    w01 = ask_parameter("Frecuencia natural ω01", w01);
    
    std::cout << "\n--- PARÁMETROS DEL OSCILADOR 2 ---" << std::endl;
    mu2 = ask_parameter("Parámetro de no linealidad μ2", mu2);
    w02 = ask_parameter("Frecuencia natural ω02", w02);
    
    std::cout << "\n--- PARÁMETROS DE ACOPLAMIENTO ---" << std::endl;
    k = ask_parameter("Constante de acoplamiento k", k);
    
    std::cout << "\n--- PARÁMETROS TEMPORALES ---" << std::endl;
    t0 = ask_parameter("Tiempo inicial t0", t0);
    tf = ask_parameter("Tiempo final tf", tf);
    dt = ask_parameter("Paso de tiempo dt", dt);
    
    std::cout << "\n--- CONDICIONES INICIALES OSCILADOR 1 ---" << std::endl;
    x10 = ask_parameter("Posición inicial x10", x10);
    v10 = ask_parameter("Velocidad inicial v10", v10);
    
    std::cout << "\n--- CONDICIONES INICIALES OSCILADOR 2 ---" << std::endl;
    x20 = ask_parameter("Posición inicial x20", x20);
    v20 = ask_parameter("Velocidad inicial v20", v20);
    
    std::cout << "\n--- RESUMEN DE PARÁMETROS ---" << std::endl;
    std::cout << "Oscilador 1: μ1=" << mu1 << ", ω01=" << w01 << std::endl;
    std::cout << "Oscilador 2: μ2=" << mu2 << ", ω02=" << w02 << std::endl;
    std::cout << "Acoplamiento: k=" << k << std::endl;
    std::cout << "Tiempo: t0=" << t0 << ", tf=" << tf << ", dt=" << dt << std::endl;
    std::cout << "Condiciones iniciales: " << std::endl;
    std::cout << "  Osc1: x10=" << x10 << ", v10=" << v10 << std::endl;
    std::cout << "  Osc2: x20=" << x20 << ", v20=" << v20 << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

int main(int argc, char **argv) {
    // Obtener rutas importantes
    std::filesystem::path current_dir = std::filesystem::current_path();
    std::filesystem::path project_root = current_dir.parent_path();
    std::filesystem::path results_dir = project_root / "results";
    
    // Crear directorio de resultados
    if (!std::filesystem::exists(results_dir)) {
        std::filesystem::create_directories(results_dir);
    }
    
    // Parámetros por defecto
    double mu1 = 1.0, w01 = 1.0, mu2 = 1.0, w02 = 1.0, k = 0.5;
    double t0 = 0.0, tf = 200.0, dt = 0.01;
    double x10 = 1.0, v10 = 0.0, x20 = 0.5, v20 = 0.0;

    // Leer parámetros de línea de comandos
    mu1 = get_arg_or_default(argc, argv, "--mu1", mu1);
    w01 = get_arg_or_default(argc, argv, "--w01", w01);
    mu2 = get_arg_or_default(argc, argv, "--mu2", mu2);
    w02 = get_arg_or_default(argc, argv, "--w02", w02);
    k = get_arg_or_default(argc, argv, "--k", k);
    t0 = get_arg_or_default(argc, argv, "--t0", t0);
    tf = get_arg_or_default(argc, argv, "--tf", tf);
    dt = get_arg_or_default(argc, argv, "--dt", dt);
    x10 = get_arg_or_default(argc, argv, "--x10", x10);
    v10 = get_arg_or_default(argc, argv, "--v10", v10);
    x20 = get_arg_or_default(argc, argv, "--x20", x20);
    v20 = get_arg_or_default(argc, argv, "--v20", v20);

    // Modo interactivo
    bool interactive = flag_present(argc, argv, "--interactive");
    if (interactive || argc == 1) {
        interactive_mode(mu1, w01, mu2, w02, k, t0, tf, dt, x10, v10, x20, v20);
    }

    // Flags de funcionalidades
    bool auto_all = flag_present(argc, argv, "--auto");
    bool do_validate = flag_present(argc, argv, "--validate_dt");
    bool do_poincare = flag_present(argc, argv, "--poincare");
    bool do_lyap = flag_present(argc, argv, "--lyapunov");
    bool do_gif = flag_present(argc, argv, "--gif");

    // Si --auto está activado, activamos todas las opciones
    if (auto_all) {
        do_validate = true;
        do_poincare = true;
        do_lyap = true;
        do_gif = true;
    }

    Sistema S(mu1, w01, mu2, w02, k);

    std::cout << "\n=== INICIANDO SIMULACIÓN ===" << std::endl;
    std::cout << "Van der Pol acoplados (RK4)" << std::endl;
    std::cout << "Parámetros:" << std::endl;
    std::cout << "  Osc1: μ1=" << mu1 << ", ω01=" << w01 << std::endl;
    std::cout << "  Osc2: μ2=" << mu2 << ", ω02=" << w02 << std::endl;
    std::cout << "  Acoplamiento: k=" << k << std::endl;
    std::cout << "  Tiempo: [" << t0 << ", " << tf << "], dt=" << dt << std::endl;
    std::cout << "  Condiciones iniciales:" << std::endl;
    std::cout << "    Osc1: x10=" << x10 << ", v10=" << v10 << std::endl;
    std::cout << "    Osc2: x20=" << x20 << ", v20=" << v20 << std::endl;
    std::cout << "Resultados en: " << results_dir << std::endl;

    // Todas las salidas van a results/ en la raíz del proyecto
    std::string outfile = (results_dir / "datos.txt").string();

    // 1. Simulación principal
    std::cout << "\n1. Ejecutando simulación principal..." << std::endl;
    S.simulate(t0, tf, dt, outfile, x10, v10, x20, v20);
    std::cout << "✓ Datos de simulación guardados en: " << outfile << std::endl;

    // 2. Validación de dt (si se solicita)
    if (do_validate) {
        std::string dtfile = (results_dir / "dt_validation.txt").string();
        std::cout << "\n2. Validando paso de tiempo..." << std::endl;
        S.validate_dt(t0, tf, dt, dtfile, x10, v10, x20, v20);
        std::cout << "✓ Validación de dt guardada en: " << dtfile << std::endl;
    }

    // 3. Mapa de Poincaré (si se solicita)
    if (do_poincare) {
        std::string poinfile = (results_dir / "poincare.txt").string();
        std::cout << "\n3. Generando mapa de Poincaré..." << std::endl;
        S.generate_poincare(t0, tf, dt, poinfile, x10, v10, x20, v20);
        std::cout << "✓ Mapa de Poincaré guardado en: " << poinfile << std::endl;
    }

    // 4. Exponentes de Lyapunov (si se solicita)
    if (do_lyap) {
        std::string lyap_prog = (results_dir / "lyapunov_progress.txt").string();
        std::string lyap_final = (results_dir / "lyapunov_final.txt").string();
        double renorm_time = get_arg_or_default(argc, argv, "--renorm", 1.0);
        std::cout << "\n4. Calculando exponentes de Lyapunov..." << std::endl;
        std::cout << "   (Esto puede tomar varios minutos)" << std::endl;
        S.compute_lyapunov(t0, tf, dt, renorm_time, lyap_prog, lyap_final, x10, v10, x20, v20, 1e-8);
        std::cout << "✓ Exponentes de Lyapunov guardados en:" << std::endl;
        std::cout << "   - Progreso: " << lyap_prog << std::endl;
        std::cout << "   - Resultado final: " << lyap_final << std::endl;
    }

    // 5. Generación automática de gráficas y GIF
    std::filesystem::path plot_script = project_root / "scripts" / "python" / "plot.py";
    
    if (std::filesystem::exists(plot_script)) {
        std::string cmd = "cd \"" + project_root.string() + "\" && python3 scripts/python/plot.py \"";
        cmd += outfile + "\"";
        
        if (do_gif) {
            cmd += " --gif";
        }
        
        std::cout << "\n5. Generando gráficas y análisis..." << std::endl;
        std::cout << "   Comando: " << cmd << std::endl;
        
        int ret = std::system(cmd.c_str());
        if (ret != 0) {
            std::cerr << "✗ Error en generación de gráficas. Código: " << ret << std::endl;
            std::cerr << "  Verifique que tenga instalado: python3, numpy, matplotlib, pillow" << std::endl;
        } else {
            std::cout << "✓ Gráficas generadas exitosamente" << std::endl;
        }
    } else {
        std::cerr << "✗ Script de graficado no encontrado: " << plot_script << std::endl;
    }

    std::cout << "\n=== SIMULACIÓN COMPLETADA ===" << std::endl;
    std::cout << "Resultados guardados en: " << results_dir << std::endl;
    
    // Mostrar resumen de archivos generados
    std::cout << "\nArchivos generados:" << std::endl;
    int file_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(results_dir)) {
        if (entry.is_regular_file()) {
            std::cout << "  - " << entry.path().filename() << std::endl;
            file_count++;
        }
    }
    std::cout << "Total: " << file_count << " archivos" << std::endl;

    return 0;
}