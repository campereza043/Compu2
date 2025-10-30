#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "Sistema.h"

/*
 * Uso:
 * ./vanderpol [--auto] [--validate_dt] [--poincare] [--lyapunov] plus params like before
 *
 * --auto : corre todo (simulate + plots)
 * --validate_dt : realiza validación de dt y escribe results/dt_validation.txt
 * --poincare : genera results/poincare.txt
 * --lyapunov : calcula Lyapunov y guarda results/lyapunov_progress.txt y results/lyapunov_final.txt
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

    std::string outdir = get_arg_or_default_str(argc, argv, "--outdir", "results");
    std::filesystem::create_directories(outdir);
    std::string outfile = get_arg_or_default_str(argc, argv, "--out", outdir + "/datos.txt");

    bool auto_all = flag_present(argc, argv, "--auto");
    bool do_validate = flag_present(argc, argv, "--validate_dt");
    bool do_poincare = flag_present(argc, argv, "--poincare");
    bool do_lyap = flag_present(argc, argv, "--lyapunov");

    Sistema S(mu1, w01, mu2, w02, k);

    std::cout << "Simulando Van der Pol acoplados (RK4) t=[" << t0 << "," << tf << "] dt=" << dt
              << " k=" << k << " mu1=" << mu1 << " mu2=" << mu2 << "\n";

    S.simulate(t0, tf, dt, outfile, x10, v10, x20, v20);
    std::cout << "Datos guardados en: " << outfile << std::endl;

    if (do_validate || auto_all) {
        std::string dtfile = outdir + "/dt_validation.txt";
        std::cout << "Validando dt -> " << dtfile << std::endl;
        S.validate_dt(t0, tf, dt, dtfile, x10, v10, x20, v20);
    }

    if (do_poincare || auto_all) {
        std::string poinfile = outdir + "/poincare.txt";
        std::cout << "Generando Poincaré -> " << poinfile << std::endl;
        S.generate_poincare(t0, tf, dt, poinfile, x10, v10, x20, v20);
    }

    if (do_lyap || auto_all) {
        std::string lyap_prog = outdir + "/lyapunov_progress.txt";
        std::string lyap_final = outdir + "/lyapunov_final.txt";
        double renorm_time = get_arg_or_default(argc, argv, "--renorm", 1.0); // segundos
        std::cout << "Calculando Lyapunov (renorm_time=" << renorm_time << ") -> " << lyap_final << std::endl;
        S.compute_lyapunov(t0, tf, dt, renorm_time, lyap_prog, lyap_final, x10, v10, x20, v20, 1e-8);
    }

    // llamar al script de python para graficar automáticamente si --auto o --plot
    if (auto_all || flag_present(argc, argv, "--plot")) {
        std::string cmd = "python3 scripts/python/plot.py " + outfile;
        std::cout << "Lanzando script de graficado: " << cmd << std::endl;
        int ret = std::system(cmd.c_str());
        if (ret != 0) {
            std::cerr << "Advertencia: el script de graficado devolvió código distinto de cero.\n";
        } else {
            std::cout << "Figuras generadas en carpeta: " << outdir << std::endl;
        }
    }

    return 0;
}
