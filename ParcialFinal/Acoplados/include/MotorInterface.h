#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

// Interfaz mínima para un motor (partícula 1D)
class MotorInterface {
public:
    virtual void Inicie(double x0, double v0) = 0;
    // Calcula la fuerza interna (potencial, driving, fricción, ruido NO incluye acoplamiento)
    // Debe devolver la fuerza total que actúa sobre la partícula debida a su modelo (sinacoplamiento)
    virtual double computeInternalForce(double t) = 0;
    virtual double X() const = 0;
    virtual double V() const = 0;
    virtual void SetX(double x) = 0;
    virtual void SetV(double v) = 0;
    // Energía interna (potencial + cinética local si lo requiere)
    virtual double getKineticEnergy() const = 0;
    virtual double getPotentialEnergy() const = 0;
    virtual ~MotorInterface() {}
};

#endif // MOTOR_INTERFACE_H
