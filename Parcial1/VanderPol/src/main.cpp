#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "Sistema.h"

/*
 * Main con parsing de parámetros por línea de comandos (forma simple).
 *
 * Uso ejemplo:
 * ./vanderpol --mu1 1.0 --w01 1.0 --mu2 1.0 --w02 1.0 --k 0.5 --t0 0 --tf 200 --dt 0.01
 *     --x10 1.0 --v10 0.0 --x20 0.5 --v20 0.0 --out results/datos.txt
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

int main(int argc, char **argv) {
    // valores por defecto
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

    Sistema S(mu1, w01, mu2, w02, k);

    std::cout << "Simulando Van der Pol acoplados (RK4) t=[" << t0 << "," << tf << "] dt=" << dt
              << " k=" << k << " mu1=" << mu1 << " mu2=" << mu2 << "\n";

    S.simulate(t0, tf, dt, outfile, x10, v10, x20, v20);

    std::cout << "Datos guardados en: " << outfile << std::endl;
    return 0;
}
