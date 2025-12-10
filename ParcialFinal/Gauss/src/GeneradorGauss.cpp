#include "GeneradorGauss.h"
#include <cmath>
#include <fstream>
#include <stdexcept>

GeneradorGauss::GeneradorGauss(double sigma, uint32_t seed)
: sigma_(sigma), rng_(seed)
{
    if(sigma_ <= 0.0) throw std::invalid_argument("sigma debe ser > 0");
}

std::vector<double> GeneradorGauss::generar(size_t N) {
    std::vector<double> salida;
    salida.reserve(N);

    // Box-Muller produce dos gaussianas por iteración
    size_t i = 0;
    while(i < N) {
        double u1 = rng_.uniform();
        double u2 = rng_.uniform();
        // proteger contra u1==0 -> log(0)
        if(u1 <= 0.0) u1 = std::numeric_limits<double>::min();

        double r = std::sqrt(-2.0 * std::log(u1));
        double theta = 2.0 * M_PI * u2;
        double z0 = r * std::cos(theta);
        double z1 = r * std::sin(theta);

        salida.push_back(sigma_ * z0);
        i++;
        if(i < N) {
            salida.push_back(sigma_ * z1);
            i++;
        }
    }
    return salida;
}

void GeneradorGauss::guardarEnArchivo(const std::vector<double>& datos, const std::string& filename) {
    std::ofstream ofs(filename);
    if(!ofs) throw std::runtime_error("No se puede abrir archivo para escribir: " + filename);
    for(const auto &v : datos) {
        ofs << v << "\n";
    }
    ofs.close();
}
