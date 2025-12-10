#ifndef FLASHING_MOTOR_H
#define FLASHING_MOTOR_H

#include "MotorInterface.h"
#include "AsymmetricPotential.h"
#include <random>

class FlashingMotor : public MotorInterface {
public:
    // T_on, T_off controlan el ciclo de encendido/apagado
    FlashingMotor(double m = 1.0, double gamma = 0.0, double kBT = 0.0,
                  double T_on = 50.0, double T_off = 20.0,
                  const AsymmetricPotential &pot = AsymmetricPotential())
        : m(m), gamma(gamma), kBT(kBT),
          T_on(T_on), T_off(T_off), pot(pot),
          x(0.0), v(0.0), rng(1234567), uni(0.0,1.0) {}

    void Inicie(double x0, double v0) override {
        x = x0; v = v0; time_acc = 0.0;
        // estado inicial: encendido
        on = true;
        phase_time = 0.0;
    }

    // devuelve la fuerza interna (sinacoplamiento): -dU/dx si está encendido, 0 si apagado,
    // más fricción y ruido (Langevin)
    double computeInternalForce(double t) override {
        // actualizar estado de flashing según tiempos
        if (on) {
            if (phase_time >= T_on) { on = false; phase_time = 0.0; }
        } else {
            if (phase_time >= T_off) { on = true; phase_time = 0.0; }
        }
        // fuerza de potencial solo si on==true
        double Fp = 0.0;
        if (on) Fp = pot.force(x);

        // fricción
        double Ffric = -gamma * v;

        // ruido gaussiano con varianza sqrt(2*gamma*kBT/dt) -> but we don't multiply by 1/dt here,
        // integrator will handle dt. We will return the deterministic force component;
        // for convenience we approximate noise as a force term with amplitude sqrt(2*gamma*kBT)
        // and use a small dt inside the simulator to scale it correctly.
        // We'll produce a Gaussian with mean 0 and std = sqrt(2*gamma*kBT)
        double Fnoise = 0.0;
        if (kBT > 0.0 && gamma > 0.0) {
            // Box-Muller
            double u1 = uni(rng);
            double u2 = uni(rng);
            double r = std::sqrt(-2.0 * std::log(std::max(1e-15, u1)));
            double theta = 2.0*M_PI*u2;
            double g = r * std::cos(theta); // N(0,1)
            Fnoise = g * std::sqrt(2.0 * gamma * kBT);
        }

        phase_time += 0.0; // NOTE: actual time increment happens in simulator
        return Fp + Ffric + Fnoise;
    }

    double X() const override { return x; }
    double V() const override { return v; }
    void SetX(double xx) override { x = xx; }
    void SetV(double vv) override { v = vv; }

    double getKineticEnergy() const override { return 0.5 * m * v * v; }
    double getPotentialEnergy() const override { return on ? pot.U(x) : 0.0; }

    // extra: advance internal clocks (simulator must call esto cada dt)
    void advanceTime(double dt) {
        time_acc += dt;
        phase_time += dt;
    }

    double mass() const { return m; }
    bool isOn() const { return on; }
    double getGamma() const { return gamma; }
    double getKBT() const { return kBT; }

private:
    double m;
    double gamma;
    double kBT;
    double T_on;
    double T_off;
    AsymmetricPotential pot;

    double x, v;
    double time_acc = 0.0;
    double phase_time = 0.0;
    bool on = true;

    // RNG for noise
    std::mt19937 rng;
    std::uniform_real_distribution<double> uni;
};

#endif // FLASHING_MOTOR_H
