#include "ModeloMotor.h"
#include <cmath>

ModeloMotor::ModeloMotor(double masa)
: m_(masa), x_(0.0), v_(0.0), a_(0.0), A_(0.0), omega_(0.0), pot_(0.0,1.0, 0.5)
{ }

void ModeloMotor::Inicie(double x0, double v0) {
    x_ = x0;
    v_ = v0;
    // a_ debe calcularse con t = 0
    a_ = fuerza_total(x_, 0.0) / m_;
}

void ModeloMotor::setDriving(double A, double omega) {
    A_ = A;
    omega_ = omega;
}

void ModeloMotor::setPotencial(const Potencial& pot) {
    pot_ = pot;
}

double ModeloMotor::fuerza_total(double x, double t) const {
    double fpot = pot_.fuerza(x);
    double fdrive = A_ * std::sin(omega_ * t);
    return -fpot + fdrive;
}

void ModeloMotor::Paso(double dt, double t) {
    // Velocity Verlet
    // v(t + dt/2) = v(t) + a(t)*dt/2
    v_ += 0.5 * a_ * dt;

    // x(t + dt) = x(t) + v(t + dt/2)*dt
    x_ += v_ * dt;

    // calcular a(t+dt) usando fuerza total evaluada en x(t+dt), t+dt
    double a_new = fuerza_total(x_, t + dt) / m_;

    // v(t+dt) = v(t + dt/2) + a(t+dt)*dt/2
    v_ += 0.5 * a_new * dt;

    // actualizar aceleración
    a_ = a_new;
}

double ModeloMotor::X() const { return x_; }
double ModeloMotor::V() const { return v_; }
double ModeloMotor::M() const { return m_; }
