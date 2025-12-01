/**
 * @file MotorModel.h
 * @brief Modelo físico del motor molecular (Flashing Ratchet).
 */

#ifndef MOTORMODEL_H
#define MOTORMODEL_H

#include "Particle.h"
#include "Potential.h"
#include "ChemicalState.h"
#include <memory>
#include <random>

/**
 * @class MotorModel
 * @brief Combina partícula, potencial y estado químico.
 * * Resuelve la ecuación: m*a = -dU/dx - gamma*v + ruido
 */
class MotorModel {
public:
    Particle p;
    std::unique_ptr<Potential> potential;
    ChemicalState chemistry;
    
    // Parámetros Langevin
    double gamma; ///< Coeficiente de fricción
    double kBT;   ///< Energía térmica (kB * T)
    
    // Generador de números aleatorios para ruido térmico
    std::mt19937 rng;
    std::normal_distribution<double> noise_dist;

    /**
     * @brief Constructor del modelo.
     */
    MotorModel(double m, double gamma_val, double temp, double t_on, double t_off);

    /**
     * @brief Calcula todas las fuerzas sobre la partícula (Potencial + Fricción + Ruido).
     * @param dt Paso de tiempo (necesario para escalar el ruido).
     */
    void computeForces(double dt);

    /**
     * @brief Calcula la energía total del sistema (Cinética + Potencial actual).
     */
    double getTotalEnergy() const;
};

#endif