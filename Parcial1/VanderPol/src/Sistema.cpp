#include "Sistema.h"
#include <fstream>
#include <iostream>


Sistema::Sistema(double mu1, double w01, double mu2, double w02, double k_) : o1(mu1, w01), o2(mu2, w02), k(k_) {}


void Sistema::simulate(double t0, double tf, double dt, const std::string &outfile) const {
// copia local de osciladores para iterar
VanDerPol a = o1;
VanDerPol b = o2;


std::ofstream ofs(outfile);
if (!ofs) {
std::cerr << "No se pudo abrir archivo de salida: " << outfile << std::endl;
return;
}


ofs << "# t x1 v1 x2 v2" << std::endl;


double t = t0;
while (t <= tf + 1e-12) {
// escribir
ofs << t << " " << a.x << " " << a.v << " " << b.x << " " << b.v << "\n";


// calcular fuerzas de acoplamiento (lineal: k*(xj - xi))
double F1 = k * (b.x - a.x);
double F2 = k * (a.x - b.x);


// RK4 paso para ambos (use las mismas fuerzas en cada subpaso)
a.rk4_step(t, dt, F1);
b.rk4_step(t, dt, F2);


t += dt;
}


ofs.close();
}
