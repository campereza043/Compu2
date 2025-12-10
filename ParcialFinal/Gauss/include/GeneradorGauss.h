#ifndef GENERADORGAUSS_H
#define GENERADORGAUSS_H

#include <vector>
#include <string>
#include "RandomEngine.h"

/**
 * @brief GeneradorGauss: genera gaussianas usando Box-Muller.
 *
 * Constructor recibe sigma y optionally seed para RNG.
 */
class GeneradorGauss {
public:
    GeneradorGauss(double sigma = 1.0, uint32_t seed = 12345u);

    /**
     * @brief Genera N valores gaussianos (media 0) y los devuelve en un vector.
     */
    std::vector<double> generar(size_t N);

    /**
     * @brief Guarda el vector de datos en un archivo (una columna).
     */
    static void guardarEnArchivo(const std::vector<double>& datos, const std::string& filename);

private:
    double sigma_;
    RandomEngine rng_;
};

#endif // GENERADORGAUSS_H
