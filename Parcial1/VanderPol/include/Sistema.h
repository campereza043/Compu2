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
 * Además incluye utilidades para:
 *  - generar mapa de Poincaré (x1=0, v1>0)
 *  - calcular el mayor exponente de Lyapunov (Benettin)
 *  - validación de paso de tiempo (dt)
 */

class Sistema {
public:
    VanDerPol o1, o2; /**< Osciladores (parámetros µ y ω0) */
    double k;        /**< constante de acoplamiento */

    Sistema(double mu1, double w01, double mu2, double w02, double k_);

    /** Simula y guarda "t x1 v1 x2 v2" */
    void simulate(double t0, double tf, double dt, const std::string &outfile,
                  double x10 = 0.0, double v10 = 0.0, double x20 = 0.0, double v20 = 0.0) const;

    /** Genera archivo de Poincaré (cruces x1=0, v1>0) con columnas t x1 v1 x2 v2 (solo puntos de sección) */
    void generate_poincare(double t0, double tf, double dt, const std::string &outfile_poincare,
                            double x10, double v10, double x20, double v20) const;

    /**
     * Calcula el mayor exponente de Lyapunov (Benettin).
     * Guarda un registro temporal en outfile_progress (time, lambda_running) y
     * al final escribe lambda_final en outfile_final.
     */
    void compute_lyapunov(double t0, double tf, double dt, double renorm_time,
                          const std::string &outfile_progress, const std::string &outfile_final,
                          double x10, double v10, double x20, double v20, double eps0 = 1e-8) const;

    /**
     * Validación de dt: integra con dt, dt/2, dt/4 y guarda la comparación final en outfile.
     * Formato: dt x1_final x2_final | error_vs_dt (abs differences)
     */
    void validate_dt(double t0, double tf, double dt, const std::string &outfile,
                     double x10, double v10, double x20, double v20) const;

private:
    void system_derivatives(const double state[4], double deriv[4]) const;
    void rk4_step(double t, double dt, double state[4]) const;
};

#endif // SISTEMA_H
