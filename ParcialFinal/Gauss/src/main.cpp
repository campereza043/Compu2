#include <iostream>
#include <filesystem>
#include <string>
#include "GeneradorGauss.h"

/*
 * Programa principal:
 * genera 1e5 muestras gaussianas con sigma = 1/sqrt(2),
 * guarda en results/gauss.dat
 */

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    namespace fs = std::filesystem;

    // Parámetros requeridos por el enunciado:
    const size_t N = 100000;                     // 10^5
    const double sigma = 1.0 / std::sqrt(2.0);   // sigma = 1/sqrt(2)
    const uint32_t seed = 123456789u;            // semilla por defecto (puede cambiarse)

    // Crear carpeta results si no existe
    fs::create_directories("results");

    try {
        GeneradorGauss gen(sigma, seed);
        auto datos = gen.generar(N);
        const std::string outfile = "results/gauss.dat";
        gen.guardarEnArchivo(datos, outfile);
        std::cout << "Generadas " << datos.size() << " muestras gaussianas.\n";
        std::cout << "Archivo guardado en: " << outfile << "\n";
        std::cout << "Para generar la figura, ejecute: make plot (usa scripts/plot_gauss.gp)\n";
    } catch(const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
