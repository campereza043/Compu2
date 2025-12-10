/** @file Integrator.cpp */
#include "../include/Integrator.h"

void VelocityVerlet::step(MotorModel& motor, double dt) {
    // 1. Primer medio paso de velocidad
    // v(t + dt/2) = v(t) + 0.5 * a(t) * dt
    motor.p.v += 0.5 * (motor.p.f / motor.p.m) * dt;
    
    // 2. Paso completo de posición
    // x(t + dt) = x(t) + v(t + dt/2) * dt
    motor.p.x += motor.p.v * dt;
    
    // 3. Actualizar estado químico y fuerzas
    // Es crucial actualizar el estado químico aquí para que la fuerza 
    // corresponda al nuevo tiempo y posición.
    motor.chemistry.update(dt);
    motor.computeForces(dt); 
    
    // 4. Segundo medio paso de velocidad
    // v(t + dt) = v(t + dt/2) + 0.5 * a(t + dt) * dt
    motor.p.v += 0.5 * (motor.p.f / motor.p.m) * dt;
}