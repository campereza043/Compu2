#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <memory>

#include "../include/CoupledMotors.h"
#include "../include/FlashingMotor.h"
#include "../include/RockingMotor.h"

using namespace std;

// Simulador Vel Verlet para dos partículas acopladas por resorte armónico Uc = 0.5 kc (x2-x1-d0)^2
// NOTA: computeInternalForce(t) devuelve la fuerza interna NO incluida la fuerza de acoplamiento.
// El simulador se encarga de la fuerza de acoplamiento.

void run_coupled_simulation(CoupledMotors &system,
                            double dt, double tmax,
                            const string &out_traj, const string &out_energy,
                            bool noisy_phase = true) {
    auto m1 = system.M1();
    auto m2 = system.M2();
    double kc = system.getKc();
    double d0 = system.getD0();

    // masses (suponemos método mass() en las implementaciones; si no, adaptar)
    double m1_mass = 1.0, m2_mass = 1.0;
    // intentamos deducir con dynamic_cast
    {
        auto f1 = dynamic_cast<FlashingMotor*>(m1.get());
        if (f1) m1_mass = f1->mass();
        auto r2 = dynamic_cast<RockingMotor*>(m2.get());
        if (r2) m2_mass = r2->mass();
    }

    // inicial conditions
    double t = 0.0;

    // outputs
    ofstream fout_traj(out_traj);
    ofstream fout_energy(out_energy);

    fout_traj << "# t x1 v1 x2 v2\n";
    fout_energy << "# t K1 K2 U1 U2 Uc E_tot\n";
    fout_traj << fixed << setprecision(8);
    fout_energy << fixed << setprecision(8);

    // initial forces (internal)
    double x1 = m1->X();
    double v1 = m1->V();
    double x2 = m2->X();
    double v2 = m2->V();

    // compute internal forces
    double F1_int = m1->computeInternalForce(t);
    double F2_int = m2->computeInternalForce(t);

    // compute coupling force: on particle 1 it's +Fcoup (since derivative w.r.t x1)
    auto compute_coupling = [&](double x1_, double x2_) {
        double diff = x2_ - x1_ - d0;
        // potential Uc = 0.5 kc diff^2
        // force on 1 = -dUc/dx1 = -kc*(diff)*(-1) = kc*diff ? careful:
        // dUc/dx1 = kc * diff * d(diff)/dx1 = kc*diff*(-1) => force = -dUc/dx1 = kc*diff
        double F_on_1 = kc * diff;
        double F_on_2 = -kc * diff;
        return std::pair<double,double>(F_on_1, F_on_2);
    };

    auto coup = compute_coupling(x1, x2);
    double F1_coup = coup.first;
    double F2_coup = coup.second;

    // Verlet loop
    int steps = int(tmax/dt);
    for (int step = 0; step <= steps; ++step) {
        // Save every step (or pick stride)
        fout_traj << t << " " << x1 << " " << v1 << " " << x2 << " " << v2 << "\n";

        double K1 = m1->getKineticEnergy();
        double K2 = m2->getKineticEnergy();
        double U1 = m1->getPotentialEnergy();
        double U2 = m2->getPotentialEnergy();
        double Uc = 0.5 * kc * (x2 - x1 - d0) * (x2 - x1 - d0);
        double Etot = K1 + K2 + U1 + U2 + Uc;
        fout_energy << t << " " << K1 << " " << K2 << " " << U1 << " " << U2 << " " << Uc << " " << Etot << "\n";

        // --- Velocity-Verlet steps for both particles simultaneously ---
        // x(t+dt) = x + v*dt + 0.5*(F/m)*dt^2
        double a1 = (F1_int + F1_coup) / m1_mass;
        double a2 = (F2_int + F2_coup) / m2_mass;

        double x1_new = x1 + v1*dt + 0.5*a1*dt*dt;
        double x2_new = x2 + v2*dt + 0.5*a2*dt*dt;

        // provisional velocities half-step
        double v1_half = v1 + 0.5*a1*dt;
        double v2_half = v2 + 0.5*a2*dt;

        // update internal states' positions so their computeInternalForce uses updated x
        m1->SetX(x1_new);
        m2->SetX(x2_new);

        // IMPORTANT: actualizar también la velocidad interna a la velocidad de medio paso
        // para que los términos de fricción en computeInternalForce(t+dt) usen una aproximación correcta.
        m1->SetV(v1_half);
        m2->SetV(v2_half);

        // update internal states' positions so their computeInternalForce uses updated x
        m1->SetX(x1_new);
        m2->SetX(x2_new);

        // advance internal clocks (if motor has method)
        // dynamic cast to call advanceTime if exists
        if (auto f1 = dynamic_cast<FlashingMotor*>(m1.get())) f1->advanceTime(dt);
        if (auto r2 = dynamic_cast<RockingMotor*>(m2.get())) r2->advanceTime(dt);

        // compute new internal forces at t+dt
        double F1_int_new = m1->computeInternalForce(t + dt);
        double F2_int_new = m2->computeInternalForce(t + dt);

        // recompute coupling at new positions
        auto coup_new = compute_coupling(x1_new, x2_new);
        double F1_coup_new = coup_new.first;
        double F2_coup_new = coup_new.second;

        double a1_new = (F1_int_new + F1_coup_new) / m1_mass;
        double a2_new = (F2_int_new + F2_coup_new) / m2_mass;

        // finalize velocities
        double v1_new = v1_half + 0.5*a1_new*dt;
        double v2_new = v2_half + 0.5*a2_new*dt;

        // commit
        x1 = x1_new; v1 = v1_new; F1_int = F1_int_new; F1_coup = F1_coup_new;
        x2 = x2_new; v2 = v2_new; F2_int = F2_int_new; F2_coup = F2_coup_new;

        // Sincronizar velocidades finales dentro de los objetos motor para que
        // getKineticEnergy() y computeInternalForce en la siguiente iteración
        // usen las velocidades actualizadas.
        m1->SetV(v1);
        m2->SetV(v2);   
        // advance time
        t += dt;
    }

    fout_traj.close();
    fout_energy.close();
    cout << "Simulación acoplada completada.\n";
}
