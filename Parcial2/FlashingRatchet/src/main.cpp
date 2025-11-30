#include <iostream>
#include <fstream>
#include <cmath>
#include "../include/MotorModel.h"
#include "../include/Integrator.h"

int main() {
    // Parámetros
    double m = 1.0;
    double gamma = 2.0;       // Régimen sobreamortiguado
    double kBT = 0.5;         // Temperatura
    double T_on = 3.0;        
    double T_off = 3.0;       
    double dt = 0.01;
    double total_time = 500.0;

    MotorModel motor(m, gamma, kBT, T_on, T_off);
    VelocityVerlet integrator;

    // Archivo de salida en la carpeta results/
    // Nota: Asegúrate de ejecutar el binario desde 'build/'
    std::ofstream outfile("../results/datos_ratchet.csv");
    outfile << "t,x,v,state,E_total\n";

    motor.computeForces(dt);

    for (double t = 0; t < total_time; t += dt) {
        if (std::fmod(t, 0.1) < dt) { 
            outfile << t << "," 
                    << motor.p.x << "," 
                    << motor.p.v << "," 
                    << motor.chemistry.getState() << ","
                    << motor.getTotalEnergy() << "\n";
        }
        integrator.step(motor, dt);
    }

    outfile.close();
    std::cout << "Simulacion terminada. Datos en results/datos_ratchet.csv" << std::endl;
    return 0;
}