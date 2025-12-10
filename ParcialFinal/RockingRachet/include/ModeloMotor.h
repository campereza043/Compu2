#ifndef MODELO_MOTOR_H
#define MODELO_MOTOR_H

#include "Potencial.h"

class ModeloMotor {
public:
    // Constructor: masa y referencia al potencial (por valor/por copia)
    ModeloMotor(double masa = 1.0);

    // inicializa condiciónes (posición y velocidad inicial)
    void Inicie(double x0, double v0);

    // fija parámetros externos (driving)
    void setDriving(double A, double omega);

    // fija potencial
    void setPotencial(const Potencial& pot);

    // da un paso de integración dt en el tiempo t (usa velocity Verlet)
    void Paso(double dt, double t);

    // getters
    double X() const;
    double V() const;
    double M() const;

private:
    double m_;
    double x_;
    double v_;
    double a_;     // aceleración actual

    // driving params
    double A_;
    double omega_;

    Potencial pot_;

    // calcula fuerza total en x y tiempo t: Fpot(x) + A*sin(omega t)
    double fuerza_total(double x, double t) const;
};

#endif // MODELO_MOTOR_H
