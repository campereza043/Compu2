#ifndef SISTEMA_H
#define SISTEMA_H

#include <string>
#include <vector>
#include "VanDerPol.h"

/**
 * @file Sistema.h
 * @brief Sistema de dos osciladores de Van der Pol acoplados linealmente.
 * @details
 * Esta clase implementa la simulación, análisis y diagnóstico de un par de osciladores de Van der Pol con acoplamiento lineal.
 *
 * La dinámica se rige por las ecuaciones:
 * \f[
 * \begin{cases}
 * \ddot{x}_1 - \mu_1(1 - x_1^2)\dot{x}_1 + \omega_{01}^2 x_1 = k(x_2 - x_1), \\
 * \ddot{x}_2 - \mu_2(1 - x_2^2)\dot{x}_2 + \omega_{02}^2 x_2 = k(x_1 - x_2).
 * \end{cases}
 * \f]
 *
 * Incluye herramientas para:
 * - Simulación numérica (método RK4)
 * - Análisis de sincronización
 * - Cálculo del exponente de Lyapunov
 * - Validación del paso de tiempo
 * - Análisis de bifurcaciones
 *
 * @see VanDerPol
 */

class Sistema {
public:
    VanDerPol o1, o2;  /**< Osciladores individuales */
    double k;          /**< Constante de acoplamiento */

    /**
     * @brief Constructor del sistema acoplado.
     * @param mu1 Parámetro de no linealidad del oscilador 1.
     * @param w01 Frecuencia natural del oscilador 1.
     * @param mu2 Parámetro de no linealidad del oscilador 2.
     * @param w02 Frecuencia natural del oscilador 2.
     * @param k_ Constante de acoplamiento entre ambos.
     */
    Sistema(double mu1, double w01, double mu2, double w02, double k_);

    // === Métodos de simulación ===

    /**
     * @brief Simula la evolución temporal del sistema.
     * @param t0 Tiempo inicial.
     * @param tf Tiempo final.
     * @param dt Paso de integración.
     * @param outfile Archivo de salida con los resultados.
     * @param x10 Posición inicial del oscilador 1.
     * @param v10 Velocidad inicial del oscilador 1.
     * @param x20 Posición inicial del oscilador 2.
     * @param v20 Velocidad inicial del oscilador 2.
     */
    void simulate(double t0, double tf, double dt, const std::string &outfile,
                  double x10 = 0.0, double v10 = 0.0, 
                  double x20 = 0.0, double v20 = 0.0) const;

    /**
     * @brief Analiza la sincronización entre los dos osciladores.
     */
    void analyze_synchronization(double t0, double tf, double dt,
                                const std::string &outfile_sync,
                                double x10, double v10, 
                                double x20, double v20) const;

    /**
     * @brief Genera un mapa de Poincaré detectando cruces por cero.
     */
    void generate_poincare(double t0, double tf, double dt, 
                          const std::string &outfile_poincare,
                          double x10, double v10, double x20, double v20) const;

    /**
     * @brief Calcula el espectro de potencia (placeholder).
     */
    void compute_power_spectrum(const std::string &datafile,
                               const std::string &outfile_spectrum) const;

    /**
     * @brief Calcula el exponente de Lyapunov mediante el método de Benettin.
     */
    void compute_lyapunov(double t0, double tf, double dt, double renorm_time,
                         const std::string &outfile_progress, 
                         const std::string &outfile_final,
                         double x10, double v10, double x20, double v20, 
                         double eps0 = 1e-8) const;

    /**
     * @brief Verifica la convergencia temporal reduciendo el paso de integración.
     */
    void validate_dt(double t0, double tf, double dt, 
                    const std::string &outfile,
                    double x10, double v10, double x20, double v20) const;

    /**
     * @brief Realiza un análisis de bifurcación variando el parámetro μ.
     */
    void bifurcation_analysis(double mu_min, double mu_max, int steps,
                             double tf, double dt, 
                             const std::string &outfile) const;

private:
    void system_derivatives(const double state[4], double deriv[4]) const;
    void rk4_step(double t, double dt, double state[4]) const;
    void compute_jacobian(const double state[4], double J[4][4]) const;
};

#endif
