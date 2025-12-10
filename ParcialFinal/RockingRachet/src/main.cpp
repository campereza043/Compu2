#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "ModeloMotor.h"
#include "Potencial.h"

int main() {
    // Parámetros físicos y numéricos (ajusta aquí)
    double m = 1.0;
    double U0 = 1.0;
    double L = 1.0;
    double alpha = 0.8;

    double A = 0.5;            // amplitud de la fuerza driving
    double omega = 1.0;        // frecuencia angular

    double dt = 0.001;
    double Ttot = 5.0;
    int Nsteps = static_cast<int>(Ttot / dt);

    // condiciones iniciales
    double x0 = 0.9;
    double v0 = 0.1;

    // crear modelos
    Potencial pot(U0, L, alpha);
    ModeloMotor motor(m);
    motor.setPotencial(pot);
    motor.setDriving(A, omega);
    motor.Inicie(x0, v0);

    // fichero de salida
    std::ofstream fout("results/trajectory.dat");
    fout << "# t x v\n";
    fout << std::fixed << std::setprecision(8);

    // escribo estado inicial
    double t = 0.0;
    fout << t << " " << motor.X() << " " << motor.V() << "\n";

    // bucle de integración
    for (int i = 1; i <= Nsteps; ++i) {
        motor.Paso(dt, t);
        t += dt;
        // opcional: guardo cada Nskip pasos si quieres reducir tamaño
        fout << t << " " << motor.X() << " " << motor.V() << "\n";
    }

    fout.close();
    std::cout << "Simulación completa. Datos en results/trajectory.dat\n";
    return 0;
}
