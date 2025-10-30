#ifndef VANDERPOL_H
#define VANDERPOL_H

/**
 * @file VanDerPol.h
 * @brief Clase para un oscilador de Van der Pol
 * 
 * Ecuación: d²x/dt² - μ(1-x²)dx/dt + ω₀²x = F_acople
 */

class VanDerPol {
public:
    double mu;    /**< Parámetro de no linealidad */
    double w0;    /**< Frecuencia natural */

    VanDerPol(double mu_ = 1.0, double w0_ = 1.0);
    
    /**
     * @brief Calcula derivadas del oscilador Van der Pol
     * @param x Posición
     * @param v Velocidad  
     * @param F_couple Fuerza de acoplamiento
     * @param dxdt Salida: dx/dt
     * @param dvdt Salida: dv/dt
     */
    void derivatives(double x, double v, double F_couple, 
                     double &dxdt, double &dvdt) const;
};

#endif