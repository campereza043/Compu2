#ifndef BOLA_H
#define BOLA_H

#include "Caja.h"

class Bola {
private:
    double x, y;
    double vx, vy;
    double m, r;

public:
    Bola();

    void Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0);

    // Simple motion update
    void Muevase(double dt);

    // Two different methods for handling wall collisions
    void ResuelvaColisionParedesSimple(const Caja& C);
    void ResuelvaColisionParedesRobusto(const Caja& C);

    // Handles collision with another ball
    void ChoqueElastico(Bola& otra);

    // Getters
    double Getx() const { return x; }
    double Gety() const { return y; }
    double Getvx() const { return vx; }
    double Getvy() const { return vy; }
    double Getm() const { return m; }
    double Getr() const { return r; }
};

#endif
