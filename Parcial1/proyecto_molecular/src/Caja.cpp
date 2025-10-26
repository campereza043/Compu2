#include "Caja.h"
#include <stdexcept>

void Caja::Defina(double W_, double H_) {
    if (W_ <= 0 || H_ <= 0) {
        throw std::invalid_argument("Dimensiones de caja deben ser positivas");
    }
    W = W_;
    H = H_;
}

bool Caja::EstaDentro(double x, double y, double r) const {
    return (x >= r) && (x <= W - r) && (y >= r) && (y <= H - r);
}
