#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "MotorModel.h"

[cite_start]// Clase base para integradores [cite: 72]
class Integrator {
public:
    virtual void step(MotorModel& motor, double dt) = 0;
    virtual ~Integrator() {}
};

// Implementación Velocity-Verlet
class VelocityVerlet : public Integrator {
public:
    void step(MotorModel& motor, double dt) override;
};

#endif