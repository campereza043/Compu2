/** @file MotorModel.cpp */
#include "../include/MotorModel.h"
#include <cmath>

MotorModel::MotorModel(double m, double gamma_val, double temp, double t_on, double t_off)
    : p(m, 0.0, 0.0), 
      chemistry(t_on, t_off), 
      gamma(gamma_val), 
      kBT(temp),
      rng(42), // Semilla fija para reproducibilidad
      noise_dist(0.0, 1.0) {
          // Inicializar potencial Ratchet
          // L=1.0, U0=5.0 (alto para atrapar), alpha=0.2 (muy asimétrico)
          potential = std::make_unique<RatchetPotential>(1.0, 5.0, 0.2); 
      }

void MotorModel::computeForces(double dt) {
    double F_pot = 0.0;
    
    // Potencial solo actúa si state == 1 (Flashing Ratchet)
    if (chemistry.getState() == 1) {
        F_pot = potential->getForce(p.x);
    }

    // Fuerza de fricción viscosa
    double F_fric = -gamma * p.v;

    // Fuerza estocástica (Ruido térmico)
    // Desviación estándar escalada para integración discreta: sqrt(2*gamma*kBT/dt)
    double sigma_noise = std::sqrt(2.0 * gamma * kBT / dt);
    double F_noise = sigma_noise * noise_dist(rng);

    p.f = F_pot + F_fric + F_noise;
}

double MotorModel::getTotalEnergy() const {
    double kinetic = 0.5 * p.m * p.v * p.v;
    double potential_E = 0.0;
    if (chemistry.getState() == 1) {
        potential_E = potential->getEnergy(p.x);
    }
    return kinetic + potential_E;
}