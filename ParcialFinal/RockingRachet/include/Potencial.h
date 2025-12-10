#ifndef POTENCIAL_H
#define POTENCIAL_H

#include <cmath>

class Potencial {
public:
    // Agregamos alpha con un valor por defecto (ej. 0.8 para asimetría fuerte)
    Potencial(double U0, double L, double alpha = 0.8);

    double U(double x) const;
    
    // OJO: Según tu código anterior, esta función retorna dU/dx (la pendiente),
    // y el signo negativo se aplica en la ecuación de movimiento en el main.
    double fuerza(double x) const; 

private:
    double U0_;
    double L_;
    double alpha_; // Posición relativa del pico (0 < alpha < 1)
};

#endif