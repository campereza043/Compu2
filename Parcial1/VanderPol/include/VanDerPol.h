#ifndef VANDERPOL_H
#define VANDERPOL_H


/**
* @file VanDerPol.h
* @brief Clase para un oscilador de Van der Pol.
*/


class VanDerPol {
public:
/** Posición */
double x;
/** Velocidad */
double v;


/** Parámetro de no-linealidad */
double mu;
/** Frecuencia natural (ω0) */
double w0;


/** Constructor por defecto */
VanDerPol(double mu_=1.0, double w0_=1.0);


/** Inicializa estado */
void set_state(double x0, double v0);


/** Calcula derivadas: dx/dt = v, dv/dt = mu(1-x^2)v - w0^2 x + F_couple */
void derivatives(double t, double F_couple, double &dxdt, double &dvdt) const;


/** Un paso de integración RK4 con paso dt y fuerza de acoplamiento F_couple (evaluada en t) */
void rk4_step(double t, double dt, double F_couple);
};


#endif // VANDERPOL_H
