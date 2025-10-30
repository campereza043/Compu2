#ifndef SISTEMA_H
#define SISTEMA_H

#include <string>
#include <vector>
#include "VanDerPol.h"

/**
 * @file Sistema.h
 * @brief Sistema de dos osciladores Van der Pol acoplados
 * 
 * Implementa análisis de sincronización, caos y herramientas de diagnóstico
 */

class Sistema {
public:
    VanDerPol o1, o2;  /**< Osciladores individuales */
    double k;           /**< Constante de acoplamiento */

    Sistema(double mu1, double w01, double mu2, double w02, double k_);

    // Simulación básica
    void simulate(double t0, double tf, double dt, const std::string &outfile,
                  double x10 = 0.0, double v10 = 0.0, 
                  double x20 = 0.0, double v20 = 0.0) const;

    // Análisis de sincronización
    void analyze_synchronization(double t0, double tf, double dt,
                                const std::string &outfile_sync,
                                double x10, double v10, 
                                double x20, double v20) const;

    // Herramientas de diagnóstico
    void generate_poincare(double t0, double tf, double dt, 
                          const std::string &outfile_poincare,
                          double x10, double v10, double x20, double v20) const;

    void compute_power_spectrum(const std::string &datafile,
                               const std::string &outfile_spectrum) const;

    void compute_lyapunov(double t0, double tf, double dt, double renorm_time,
                         const std::string &outfile_progress, 
                         const std::string &outfile_final,
                         double x10, double v10, double x20, double v20, 
                         double eps0 = 1e-8) const;

    void validate_dt(double t0, double tf, double dt, 
                    const std::string &outfile,
                    double x10, double v10, double x20, double v20) const;

    // Estudios paramétricos
    void bifurcation_analysis(double mu_min, double mu_max, int steps,
                             double tf, double dt, 
                             const std::string &outfile) const;

private:
    void system_derivatives(const double state[4], double deriv[4]) const;
    void rk4_step(double t, double dt, double state[4]) const;
    
    // Helper para cálculo de Jacobiano
    void compute_jacobian(const double state[4], double J[4][4]) const;
};

#endif