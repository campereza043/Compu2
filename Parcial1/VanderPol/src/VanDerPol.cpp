/**
 * @file VanDerPol.cpp
 * @brief Implementación de la clase VanDerPol.
 */

#include "VanDerPol.h"

/**
 * @brief Constructor de la clase VanDerPol.
 */
VanDerPol::VanDerPol(double mu_, double w0_) : mu(mu_), w0(w0_) {}

/**
 * @brief Calcula las derivadas del sistema de Van der Pol.
 * @param x Posición.
 * @param v Velocidad.
 * @param F_couple Fuerza de acoplamiento.
 * @param dxdt Referencia donde se almacena dx/dt.
 * @param dvdt Referencia donde se almacena dv/dt.
 */
void VanDerPol::derivatives(double x, double v, double F_couple, double &dxdt, double &dvdt) const {
    dxdt = v;
    dvdt = mu * (1.0 - x * x) * v - (w0 * w0) * x + F_couple;
}

