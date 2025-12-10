#ifndef ASYMMETRIC_POTENTIAL_H
#define ASYMMETRIC_POTENTIAL_H

#include <cmath>

// Potencial periódico asimétrico simple U(x) = U0 * [sin(2πx/L) + (alpha/2) sin(4πx/L)]
// (forma fácil de ajustar asimetría con 'alpha')
class AsymmetricPotential {
public:
    AsymmetricPotential(double U0 = 1.0, double L = 1.0, double alpha = 0.0)
        : U0(U0), L(L), alpha(alpha) {}

    double U(double x) const {
        double z = 2.0*M_PI*x/L;
        return U0*(std::sin(z) + (alpha/2.0)*std::sin(2.0*z));
    }

    // fuerza = -dU/dx
    double force(double x) const {
        double z = 2.0*M_PI*x/L;
        double dUdz = std::cos(z) + alpha*std::cos(2.0*z);
        // dU/dx = dU/dz * dz/dx = dUdz * (2π/L)
        return -U0 * dUdz * (2.0*M_PI/L);
    }

    double getU0() const { return U0; }
    double getL() const { return L; }
    double getAlpha() const { return alpha; }

private:
    double U0;
    double L;
    double alpha;
};

#endif // ASYMMETRIC_POTENTIAL_H
