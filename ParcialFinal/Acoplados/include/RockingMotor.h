#ifndef ROCKING_MOTOR_H
#define ROCKING_MOTOR_H

#include "MotorInterface.h"
#include "AsymmetricPotential.h"
#include <cmath>

class RockingMotor : public MotorInterface {
public:
    // driving: amplitude A, frequency omega
    RockingMotor(double m = 1.0, double gamma = 0.0, double kBT = 0.0,
                 double A = 0.0, double omega = 0.0,
                 const AsymmetricPotential &pot = AsymmetricPotential())
        : m(m), gamma(gamma), kBT(kBT), A(A), omega(omega), pot(pot),
          x(0.0), v(0.0) {}

    void Inicie(double x0, double v0) override {
        x = x0; v = v0; time_acc = 0.0;
    }

    // fuerza interna: -dU/dx + A sin(omega t) + fricción + ruido
    double computeInternalForce(double t) override {
        double Fp = pot.force(x);
        double Fdrive = A * std::sin(omega * t);
        double Ffric = -gamma * v;

        // ruido (similar aproximación que Flashing)
        double Fnoise = 0.0;
        if (kBT > 0.0 && gamma > 0.0) {
            // uso un RNG simple; para reproducibilidad la semilla es fija según time_acc
            // pero para evitar dependencia aquí devolvemos 0 noise; el integrador puede añadir ruido si se desea exactitud estocástica
            // Sin embargo para sencillez si kBT>0 usaremos un simple ruido aleatorio por std::rand()
            double u = (double)rand() / RAND_MAX;
            double v1 = (double)rand() / RAND_MAX;
            double r = std::sqrt(-2.0 * std::log(std::max(1e-15, u)));
            double theta = 2.0*M_PI*v1;
            double g = r * std::cos(theta);
            Fnoise = g * std::sqrt(2.0 * gamma * kBT);
        }

        return Fp + Fdrive + Ffric + Fnoise;
    }

    double X() const override { return x; }
    double V() const override { return v; }
    void SetX(double xx) override { x = xx; }
    void SetV(double vv) override { v = vv; }

    double getKineticEnergy() const override { return 0.5 * m * v * v; }
    double getPotentialEnergy() const override { return pot.U(x); }

    void advanceTime(double dt) { time_acc += dt; }

    double mass() const { return m; }

private:
    double m;
    double gamma;
    double kBT;
    double A;
    double omega;
    AsymmetricPotential pot;

    double x, v;
    double time_acc = 0.0;
};

#endif // ROCKING_MOTOR_H
