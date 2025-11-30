/**
 * @file Sistema.h
 * @brief Sistema de Dinámica Molecular con Lennard-Jones.
 */

#ifndef SISTEMA_H
#define SISTEMA_H

#include "Caja.h"
#include "Bola.h"
#include <vector>
#include <fstream>
#include <string>

class Sistema {
private:
    Caja caja;
    std::vector<Bola> bolas;
    
    // Parámetros de Lennard-Jones
    double epsilon;
    double sigma;

    // Métodos privados de cálculo
    void CalculeFuerzas();
    void PasoVelocityVerlet(double dt);

public:
    Sistema(); // Constructor para valores por defecto

    void DefinaCaja(double W, double H);
    void DefinaParametrosLJ(double eps, double sig);
    void Reserve(int N);
    
    void InicialiceRejilla(double m, double r, double v_max);
    
    // Método principal de avance
    void Paso(double dt);

    // Salida de datos
    void Encabezado(std::ofstream& f);
    void Guarde(std::ofstream& f, double t);
};

#endif