#include "VanDerPol.h"

VanDerPol::VanDerPol(double mu_, double w0_) : mu(mu_), w0(w0_) {}

void VanDerPol::derivatives(double x, double v, double F_couple, double &dxdt, double &dvdt) const {
    dxdt = v;
    dvdt = mu * (1.0 - x * x) * v - (w0 * w0) * x + F_couple;
}
