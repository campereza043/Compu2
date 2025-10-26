#ifndef SISTEMA_H
#define SISTEMA_H


#include <string>
#include <vector>
#include "VanDerPol.h"


/**
* @file Sistema.h
* @brief Sistema que gestiona dos osciladores de Van der Pol acoplados por resorte lineal
*/


class Sistema {
public:
VanDerPol o1, o2;
/** constante de acoplamiento k */
double k;


Sistema(double mu1, double w01, double mu2, double w02, double k_);


/** Integrar desde t0 hasta tf con paso dt y guardar resultados en archivo */
void simulate(double t0, double tf, double dt, const std::string &outfile) const;


};


#endif // SISTEMA_H
