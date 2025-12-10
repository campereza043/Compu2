#include "Potencial.h"
#include <cmath>

// Constructor: inicializamos alpha también
Potencial::Potencial(double U0, double L, double alpha) 
    : U0_(U0), L_(L), alpha_(alpha) { 
}

double Potencial::U(double x) const {
    // 1. Reducir x al intervalo periódico [0, L)
    // Esta fórmula funciona correctamente incluso para x negativos
    double xp = x - std::floor(x / L_) * L_;
    
    // Punto donde está el pico del diente
    double x_peak = alpha_ * L_;

    // 2. Calcular U(x) por tramos
    if (xp < x_peak) {
        // Subida lineal: y = (U0 / x_peak) * xp
        return U0_ * (xp / x_peak);
    } else {
        // Bajada lineal desde U0 hasta 0
        // Pendiente negativa: -U0 / (L - x_peak)
        return U0_ * (1.0 - (xp - x_peak) / (L_ - x_peak));
    }
}

double Potencial::fuerza(double x) const {
    // Nota Importante: Tu ecuación es m*a = -dU/dx + ...
    // Tu código anterior retornaba dU/dx (la derivada positiva).
    // Mantendré esa convención aquí. Esta función retorna LA DERIVADA (pendiente).
    
    double xp = x - std::floor(x / L_) * L_;
    double x_peak = alpha_ * L_;
    
    double dUdx = 0.0;

    if (xp < x_peak) {
        // Pendiente positiva constante (Fuerza física será negativa)
        // dU/dx = U0 / (alpha * L)
        dUdx = U0_ / x_peak;
    } else {
        // Pendiente negativa constante (Fuerza física será positiva)
        // dU/dx = -U0 / ((1 - alpha) * L)
        dUdx = -U0_ / (L_ - x_peak);
    }

    return dUdx;
}