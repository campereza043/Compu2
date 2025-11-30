#ifndef MOTORMODEL_H
#define MOTORMODEL_H

#include "Particle.h"
#include "Potential.h"
#include "ChemicalState.h"
#include <memory>
#include <random>

[cite_start]// Combina partícula, potencial y estado químico [cite: 71]
class MotorModel {
public:
    Particle p;
    std::unique_ptr<Potential> potential;
    ChemicalState chemistry;
    
    // Parámetros Langevin
    double gamma; 
    double kBT;   
    
    // Generador de números aleatorios
    std::mt19937 rng;
    std::normal_distribution<double> noise_dist;

    MotorModel(double m, double gamma_val, double temp, double t_on, double t_off);

    void computeForces(double dt);
    double getTotalEnergy() const;
};

#endif