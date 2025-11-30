#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle {
    double x;
    double v;
    double m;
    double f; // Fuerza acumulada

    Particle(double mass, double x0, double v0);
};

#endif