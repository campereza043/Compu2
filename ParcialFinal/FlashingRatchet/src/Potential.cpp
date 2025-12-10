/** @file Potential.cpp */
#include "../include/Potential.h"
#include <cmath>

RatchetPotential::RatchetPotential(double period, double height, double asymmetry) 
    : L(period), U0(height), alpha(asymmetry) {}

double RatchetPotential::getEnergy(double x) const {
    // Mapear x a [0, L) correctamente incluso para x negativos
    double x_eff = x - std::floor(x / L) * L;
    double x_peak = alpha * L;

    if (x_eff < x_peak) {
        // Subida
        return (U0 / x_peak) * x_eff;
    } else {
        // Bajada
        return U0 * (L - x_eff) / (L - x_peak);
    }
}

double RatchetPotential::getForce(double x) const {
    double x_eff = x - std::floor(x / L) * L;
    double x_peak = alpha * L;

    // F = -dU/dx
    if (x_eff < x_peak) {
        return -(U0 / x_peak); 
    } else {
        return (U0 / (L - x_peak)); 
    }
}