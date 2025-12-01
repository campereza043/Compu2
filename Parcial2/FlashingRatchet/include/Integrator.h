/**
 * @file Integrator.h
 * @brief Define esquemas de integración numérica.
 */

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "MotorModel.h"

/**
 * @class Integrator
 * @brief Clase base abstracta para integradores numéricos.
 */
class Integrator {
public:
    /**
     * @brief Avanza el sistema un paso de tiempo dt.
     * @param motor Referencia al modelo del motor molecular.
     * @param dt Paso de tiempo.
     */
    virtual void step(MotorModel& motor, double dt) = 0;
    virtual ~Integrator() {}
};

/**
 * @class VelocityVerlet
 * @brief Implementación del algoritmo Velocity-Verlet.
 * * Es un integrador simpléctico de segundo orden, estable para MD.
 */
class VelocityVerlet : public Integrator {
public:
    void step(MotorModel& motor, double dt) override;
};

#endif