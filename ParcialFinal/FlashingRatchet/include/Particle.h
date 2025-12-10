/**
 * @file Particle.h
 * @brief Estructura que representa una partícula puntual.
 */

#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle {
    double x; ///< Posición
    double v; ///< Velocidad
    double m; ///< Masa
    double f; ///< Fuerza neta acumulada

    /**
     * @brief Constructor de la partícula.
     * @param mass Masa de la partícula.
     * @param x0 Posición inicial.
     * @param v0 Velocidad inicial.
     */
    Particle(double mass, double x0, double v0);
};

#endif