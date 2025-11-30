#ifndef POTENTIAL_H
#define POTENTIAL_H

[cite_start]// Clase abstracta base [cite: 69]
class Potential {
public:
    virtual double getEnergy(double x) const = 0;
    virtual double getForce(double x) const = 0;
    virtual ~Potential() {}
};

// Implementación del Potencial de Ratchet (Diente de sierra)
class RatchetPotential : public Potential {
private:
    double L;      // Periodo
    double U0;     // Altura
    double alpha;  // Asimetría (0 < alpha < 1)

public:
    RatchetPotential(double period, double height, double asymmetry);
    double getEnergy(double x) const override;
    double getForce(double x) const override;
};

#endif