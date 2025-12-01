/**
 * @file Potential.h
 * @brief Define potenciales de interacción.
 */

#ifndef POTENTIAL_H
#define POTENTIAL_H

/**
 * @class Potential
 * @brief Clase base abstracta para potenciales 1D.
 */
class Potential {
public:
    virtual double getEnergy(double x) const = 0;
    virtual double getForce(double x) const = 0;
    virtual ~Potential() {}
};

/**
 * @class RatchetPotential
 * @brief Potencial periódico asimétrico (diente de sierra).
 * * U(x) periódica con periodo L y asimetría alpha.
 */
class RatchetPotential : public Potential {
private:
    double L;      ///< Periodo espacial
    double U0;     ///< Altura de la barrera de potencial
    double alpha;  ///< Factor de asimetría (0 < alpha < 1)

public:
    RatchetPotential(double period, double height, double asymmetry);
    double getEnergy(double x) const override;
    double getForce(double x) const override;
};

#endif