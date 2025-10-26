#include <iostream>
#include <string>
#include <filesystem>
#include "Sistema.h"


int main(int argc, char **argv) {
// parámetros por defecto (ejemplo)
double mu1 = 1.0, w01 = 1.0;
double mu2 = 1.0, w02 = 1.0;
double k = 0.5;
double t0 = 0.0, tf = 200.0, dt = 0.01;


// condición iniciales
double x10 = 1.0, v10 = 0.0;
double x20 = 0.5, v20 = 0.0;


std::string outdir = "../results";
std::filesystem::create_directories(outdir);
std::string outfile = outdir + "/datos.txt";


Sistema S(mu1, w01, mu2, w02, k);
S.o1.set_state(x10, v10);
S.o2.set_state(x20, v20);


std::cout << "Simulando... t=[" << t0 << "," << tf << "] dt=" << dt << " k=" << k << "\n";
S.simulate(t0, tf, dt, outfile);
std::cout << "Datos guardados en: " << outfile << std::endl;
return 0;
}
