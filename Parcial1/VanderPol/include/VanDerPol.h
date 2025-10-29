#ifndef VANDERPOL_H
#define VANDERPOL_H

/**
 * @file VanDerPol.h
 * @brief Clase para un oscilador de Van der Pol (contiene parámetros, utilitario de derivadas).
 *
 * Esta clase contiene sólo los parámetros y un método que calcula las derivadas
 * (dx/dt, dv/dt) para un oscilador dado, sin gestionar el paso de integración.
 */

class VanDerPol {
public:
    /** Parámetro de no-linealidad µ */
    double mu;
    /** Frecuencia natural (ω0) */
    double w0;

    /** Constructor por defecto (parametrizable) */
    VanDerPol(double mu_ = 1.0, double w0_ = 1.0);

    /**
     * @brief Calcula las derivadas de un solo oscilador Van der Pol sin acoplamiento.
     * @param x posición
     * @param v velocidad
     * @param F_couple fuerza externa (acoplamiento k*(x_j - x_i))
     * @param dxdt salida dx/dt
     * @param dvdt salida dv/dt
     */
    void derivatives(double x, double v, double F_couple, double &dxdt, double &dvdt) const;
};

#endif // VANDERPOL_H
