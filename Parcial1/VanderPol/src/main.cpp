#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
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

int main(int argc, char **argv) {
    // Obtener rutas importantes - VERSIÓN CORREGIDA
    std::filesystem::path current_dir = std::filesystem::current_path();
    std::filesystem::path project_root = current_dir.parent_path();
    std::filesystem::path results_dir = project_root / "results";
    
    std::cout << "Directorio actual: " << current_dir << std::endl;
    std::cout << "Raíz del proyecto: " << project_root << std::endl;
    std::cout << "Directorio de resultados: " << results_dir << std::endl;
    
    // Crear directorio de resultados
    if (!std::filesystem::exists(results_dir)) {
        std::filesystem::create_directories(results_dir);
        std::cout << "✓ Directorio results creado: " << results_dir << std::endl;
    }
    
    // parámetros por defecto
    double mu1 = get_arg_or_default(argc, argv, "--mu1", 1.0);
    double w01 = get_arg_or_default(argc, argv, "--w01", 1.0);
    double mu2 = get_arg_or_default(argc, argv, "--mu2", 1.0);
    double w02 = get_arg_or_default(argc, argv, "--w02", 1.0);
    double k = get_arg_or_default(argc, argv, "--k", 0.5);

    double t0 = get_arg_or_default(argc, argv, "--t0", 0.0);
    double tf = get_arg_or_default(argc, argv, "--tf", 200.0);
    double dt = get_arg_or_default(argc, argv, "--dt", 0.01);

    double x10 = get_arg_or_default(argc, argv, "--x10", 1.0);
    double v10 = get_arg_or_default(argc, argv, "--v10", 0.0);
    double x20 = get_arg_or_default(argc, argv, "--x20", 0.5);
    double v20 = get_arg_or_default(argc, argv, "--v20", 0.0);

    // Todas las salidas van a results/ en la raíz del proyecto
    std::string outfile = (results_dir / "datos.txt").string();

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

    std::cout << "Simulando Van der Pol acoplados (RK4) t=[" << t0 << "," << tf << "] dt=" << dt
              << " k=" << k << " mu1=" << mu1 << " mu2=" << mu2 << "\n";
    std::cout << "Resultados en: " << results_dir << std::endl;

    // 1. Simulación principal
    S.simulate(t0, tf, dt, outfile, x10, v10, x20, v20);
    std::cout << "✓ Datos de simulación guardados en: " << outfile << std::endl;

    // 2. Validación de dt (si se solicita)
    if (do_validate) {
        std::string dtfile = (results_dir / "dt_validation.txt").string();
        std::cout << "✓ Validando dt -> " << dtfile << std::endl;
        S.validate_dt(t0, tf, dt, dtfile, x10, v10, x20, v20);
    }

    // 3. Mapa de Poincaré (si se solicita)
    if (do_poincare) {
        std::string poinfile = (results_dir / "poincare.txt").string();
        std::cout << "✓ Generando Poincaré -> " << poinfile << std::endl;
        S.generate_poincare(t0, tf, dt, poinfile, x10, v10, x20, v20);
    }

    // 4. Exponentes de Lyapunov (si se solicita)
    if (do_lyap) {
        std::string lyap_prog = (results_dir / "lyapunov_progress.txt").string();
        std::string lyap_final = (results_dir / "lyapunov_final.txt").string();
        double renorm_time = get_arg_or_default(argc, argv, "--renorm", 1.0);
        std::cout << "✓ Calculando Lyapunov (renorm_time=" << renorm_time << ") -> " << lyap_final << std::endl;
        S.compute_lyapunov(t0, tf, dt, renorm_time, lyap_prog, lyap_final, x10, v10, x20, v20, 1e-8);
    }

    // 5. Generación automática de gráficas y GIF
    std::filesystem::path plot_script = project_root / "scripts" / "python" / "plot.py";
    
    if (std::filesystem::exists(plot_script)) {
        std::string cmd = "cd \"" + project_root.string() + "\" && python3 scripts/python/plot.py \"";
        cmd += outfile + "\"";
        
        if (do_gif) {
            cmd += " --gif";
        }
        
        std::cout << "✓ Generando gráficas: " << cmd << std::endl;
        
        int ret = std::system(cmd.c_str());
        if (ret != 0) {
            std::cerr << "✗ Error en generación de gráficas. Código: " << ret << std::endl;
            std::cerr << "  Verifique que tenga instalado: python3, numpy, matplotlib, pillow" << std::endl;
        } else {
            std::cout << "✓ Gráficas generadas exitosamente en: " << results_dir << std::endl;
        }
    } else {
        std::cerr << "✗ Script de graficado no encontrado: " << plot_script << std::endl;
    }

    std::cout << "\n=== SIMULACIÓN COMPLETADA ===" << std::endl;
    std::cout << "Resultados en: " << results_dir << std::endl;
    
    // Mostrar resumen de archivos generados
    std::cout << "\nArchivos generados:" << std::endl;
    for (const auto& entry : std::filesystem::directory_iterator(results_dir)) {
        if (entry.is_regular_file()) {
            std::cout << "  - " << entry.path().filename() << std::endl;
        }
    }

    return 0;
}