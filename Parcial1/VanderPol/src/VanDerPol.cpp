#include "VanDerPol.h"
#include <cmath>


VanDerPol::VanDerPol(double mu_, double w0_) : x(0.0), v(0.0), mu(mu_), w0(w0_) {}


void VanDerPol::set_state(double x0, double v0) {
x = x0; v = v0;
}


void VanDerPol::derivatives(double /*t*/, double F_couple, double &dxdt, double &dvdt) const {
dxdt = v;
dvdt = mu * (1.0 - x * x) * v - (w0 * w0) * x + F_couple;
}


void VanDerPol::rk4_step(double t, double dt, double F_couple) {
double k1x, k1v;
double k2x, k2v;
double k3x, k3v;
double k4x, k4v;


derivatives(t, F_couple, k1x, k1v);
// mid 1
double x_temp = x + 0.5 * dt * k1x;
double v_temp = v + 0.5 * dt * k1v;
// temporary oscillator for mid evaluation (we only need derivatives formula)
double dxdt_mid, dvdt_mid;
// For the mid evaluations we assume F_couple stays constant over the small dt (explicit RK4)
// but the state used is the mid-state
// compute k2
dxdt_mid = v_temp;
dvdt_mid = mu * (1.0 - x_temp * x_temp) * v_temp - (w0 * w0) * x_temp + F_couple;
k2x = dxdt_mid; k2v = dvdt_mid;


// k3
x_temp = x + 0.5 * dt * k2x;
v_temp = v + 0.5 * dt * k2v;
dxdt_mid = v_temp;
dvdt_mid = mu * (1.0 - x_temp * x_temp) * v_temp - (w0 * w0) * x_temp + F_couple;
k3x = dxdt_mid; k3v = dvdt_mid;


// k4
x_temp = x + dt * k3x;
v_temp = v + dt * k3v;
dxdt_mid = v_temp;
dvdt_mid = mu * (1.0 - x_temp * x_temp) * v_temp - (w0 * w0) * x_temp + F_couple;
k4x = dxdt_mid; k4v = dvdt_mid;


x += dt * (k1x + 2.0 * k2x + 2.0 * k3x + k4x) / 6.0;
v += dt * (k1v + 2.0 * k2v + 2.0 * k3v + k4v) / 6.0;
}
