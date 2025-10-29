#ifndef SISTEMA_H
#define SISTEMA_H

#include <string>
#include "VanDerPol.h"

/**
 * @file Sistema.h
 * @brief Sistema que gestiona dos osciladores de Van der Pol acoplados por resorte lineal.
 *
 * Implementa RK4 para el sistema acoplado de 4 variables:
 *  x1, v1, x2, v2
 *
 * Salida de datos: columnas t x1 v1 x2 v2
 */

class Sistema {
public:
    VanDerPol o1, o2; /**< Osciladores (parámetros µ y ω0) */
    double k;        /**< constante de acoplamiento */

    Sistema(double mu1, double w01, double mu2, double w02, double k_);

    /**
     * @brief Simula desde t0 a tf con paso dt y guarda datos en outfile.
     * Formato: columnas "t x1 v1 x2 v2"
     */
    void simulate(double t0, double tf, double dt, const std::string &outfile,
                  double x10 = 0.0, double v10 = 0.0, double x20 = 0.0, double v20 = 0.0) const;

private:
    /**
     * @brief Calcula las derivadas del sistema acoplado en el estado dado.
     * @param state arreglo de 4 elementos: [x1, v1, x2, v2]
     * @param deriv arreglo de salida con las 4 derivadas
     */
    void system_derivatives(const double state[4], double deriv[4]) const;

    /**
     * @brief Un paso RK4 para el sistema acoplado (actualiza state in-place).
     * @param t tiempo actual (no necesario en este sistema autónomo salvo si se añade forzamiento)
     * @param dt paso
     * @param state arreglo de 4 elementos (x1,v1,x2,v2) modificado in-place
     */
    void rk4_step(double t, double dt, double state[4]) const;
};

#endif // SISTEMA_H
