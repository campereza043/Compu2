#include "Sistema.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>

Sistema::Sistema(double mu1, double w01, double mu2, double w02, double k_) :
    o1(mu1, w01), o2(mu2, w02), k(k_) {}


void Sistema::system_derivatives(const double state[4], double deriv[4]) const {
    // state = [x1, v1, x2, v2]
    double x1 = state[0], v1 = state[1], x2 = state[2], v2 = state[3];

    // fuerzas de acoplamiento lineal: k*(xj - xi)
    double F1 = k * (x2 - x1);
    double F2 = k * (x1 - x2);

    double dx1dt, dv1dt, dx2dt, dv2dt;
    o1.derivatives(x1, v1, F1, dx1dt, dv1dt);
    o2.derivatives(x2, v2, F2, dx2dt, dv2dt);

    deriv[0] = dx1dt;
    deriv[1] = dv1dt;
    deriv[2] = dx2dt;
    deriv[3] = dv2dt;
}


void Sistema::rk4_step(double /*t*/, double dt, double state[4]) const {
    double k1[4], k2[4], k3[4], k4[4], tmp[4];

    system_derivatives(state, k1);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + 0.5 * dt * k1[i];
    system_derivatives(tmp, k2);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + 0.5 * dt * k2[i];
    system_derivatives(tmp, k3);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + dt * k3[i];
    system_derivatives(tmp, k4);

    for (int i = 0; i < 4; ++i) state[i] += dt * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]) / 6.0;
}


void Sistema::simulate(double t0, double tf, double dt, const std::string &outfile,
                       double x10, double v10, double x20, double v20) const {

    std::filesystem::path outpath(outfile);
    std::filesystem::create_directories(outpath.parent_path());

    std::ofstream ofs(outfile);
    if (!ofs) {
        std::cerr << "No se pudo abrir archivo de salida: " << outfile << std::endl;
        return;
    }

    ofs << "# t x1 v1 x2 v2" << std::endl;
    ofs << std::fixed << std::setprecision(8);

    double state[4];
    state[0] = x10;
    state[1] = v10;
    state[2] = x20;
    state[3] = v20;

    double t = t0;
    // integración con paso constante
    while (t <= tf + 1e-12) {
        ofs << t << " " << state[0] << " " << state[1] << " " << state[2] << " " << state[3] << "\n";
        rk4_step(t, dt, state);
        t += dt;
    }
    ofs.close();
}
