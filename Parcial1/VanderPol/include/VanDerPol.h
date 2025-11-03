#ifndef VANDERPOL_H
#define VANDERPOL_H

/**
 * @file VanDerPol.h
 * @brief Clase que implementa un oscilador de Van der Pol.
 * @details
 * Este archivo define la clase `VanDerPol`, la cual modela un oscilador
 * no lineal del tipo Van der Pol descrito por la ecuación diferencial:
 * \f[
 * \frac{d^2x}{dt^2} - \mu (1 - x^2) \frac{dx}{dt} + \omega_0^2 x = F_\text{acople}
 * \f]
 * donde:
 * - \f$\mu\f$ controla la no linealidad y el amortiguamiento.
 * - \f$\omega_0\f$ es la frecuencia natural.
 * - \f$F_\text{acople}\f$ representa una fuerza externa o de acoplamiento.
 *
 * Esta clase se utiliza como componente base del sistema de osciladores acoplados.
 * 
 * @see Sistema
 * @version 1.0
 * @date 2025
 * @author Camila Pérez, Sergio Carasquilla
 */

class VanDerPol {
public:
    double mu;    /**< Parámetro de no linealidad */
    double w0;    /**< Frecuencia natural */

    /**
     * @brief Constructor de la clase VanDerPol.
     * @param mu_ Valor del parámetro de no linealidad μ.
     * @param w0_ Frecuencia natural ω₀ del oscilador.
     */
    VanDerPol(double mu_ = 1.0, double w0_ = 1.0);
    
    /**
     * @brief Calcula las derivadas del oscilador de Van der Pol.
     * @details
     * Evalúa el sistema de ecuaciones de primer orden equivalente:
     * \f[
     * \begin{cases}
     * \dot{x} = v, \\
     * \dot{v} = \mu(1 - x^2)v - \omega_0^2 x + F_\text{acople}.
     * \end{cases}
     * \f]
     * @param x Posición actual del oscilador.
     * @param v Velocidad actual del oscilador.
     * @param F_couple Fuerza de acoplamiento aplicada.
     * @param dxdt Salida: derivada de x respecto al tiempo.
     * @param dvdt Salida: derivada de v respecto al tiempo.
     */
    void derivatives(double x, double v, double F_couple, 
                     double &dxdt, double &dvdt) const;
};

#endif
