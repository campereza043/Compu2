#include "../include/Integrator.h"

void VelocityVerlet::step(MotorModel& motor, double dt) {
    // 1. v(t + 0.5dt)
    motor.p.v += 0.5 * (motor.p.f / motor.p.m) * dt;
    
    // 2. x(t + dt)
    motor.p.x += motor.p.v * dt;
    
    // 3. Actualizar estado y fuerzas
    motor.chemistry.update(dt);
    motor.computeForces(dt); 
    
    // 4. v(t + dt)
    motor.p.v += 0.5 * (motor.p.f / motor.p.m) * dt;
}