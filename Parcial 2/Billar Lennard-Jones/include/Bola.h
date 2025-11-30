/**
 * @file Bola.h
 * @brief Clase Bola actualizada para Dinámica Molecular con fuerzas.
 */

#ifndef BOLA_H
#define BOLA_H

#include "Caja.h"

class Bola {
private:
    double x, y;       ///< Posición
    double vx, vy;     ///< Velocidad
    double fx, fy;     ///< Fuerza acumulada (Nueva)
    double m;          ///< Masa
    double r;          ///< Radio (para colisión con paredes y dibujo)

public:
    Bola();
    void Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0);

    /**
     * @brief Actualiza la posición (Paso 1 de Velocity-Verlet).
     * r(t+dt) = r(t) + v(t)dt + 0.5*a(t)dt^2
     * Pero en Velocity Verlet estándar se suele hacer: r += v * dt + 0.5 * a * dt^2
     * O el esquema: v(1/2) = v + 0.5*a*dt; r(new) = r + v(1/2)*dt.
     * Aquí usaremos métodos granulares para que Sistema controle el orden.
     */
    void Mueva_r(double dt);

    /**
     * @brief Actualiza la velocidad (Paso de medio paso o paso completo).
     * @param dt Paso de tiempo (puede ser dt o dt/2).
     */
    void Mueva_v(double dt);

    // Gestión de fuerzas
    void ResetFuerza();
    void AgregueFuerza(double Fx, double Fy);

    // Colisiones con paredes (Reflexión elástica)
    void ResuelvaColisionParedesRobusto(const Caja& C);

    // Getters
    double Getx() const { return x; }
    double Gety() const { return y; }
    double Getvx() const { return vx; }
    double Getvy() const { return vy; }
    double GetFx() const { return fx; } // Debug
    double GetFy() const { return fy; } // Debug
    double Getm() const { return m; }
    double Getr() const { return r; }
};

#endif