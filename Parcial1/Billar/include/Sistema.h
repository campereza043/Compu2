#ifndef SISTEMA_H
#define SISTEMA_H

#include "Caja.h"
#include "Bola.h"
#include <vector>
#include <fstream>
#include <string>

// NEW: Enum to represent the choice of integrator
enum class Integrador { Euler, Verlet };

class Sistema {
private:
    Caja caja;
    std::vector<Bola> bolas;
    Integrador integrador_actual = Integrador::Verlet; // Default to the best one

    // Private methods for each integration step
    void PasoEuler(double dt);
    void PasoVerlet(double dt);

public:
    void DefinaCaja(double W, double H);
    void Reserve(int N);
    void InicialiceRejilla(double m, double r, double v_max, bool alterna = false);

    // Method to select the integrator from main
    void SeleccioneIntegrador(const std::string& nombre);

    // Main step function that calls the selected private method
    void Paso(double dt);

    // File I/O methods
    void Encabezado(std::ofstream& f);
    void Guarde(std::ofstream& f, double t);
};

#endif
