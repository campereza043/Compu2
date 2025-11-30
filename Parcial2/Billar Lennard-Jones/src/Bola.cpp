/**
 * @file Bola.cpp
 * @brief Implementación de Bola para LJ.
 */

#include "Bola.h"
#include <cmath>

Bola::Bola() : x(0), y(0), vx(0), vy(0), fx(0), fy(0), m(1.0), r(0.2) {}

void Bola::Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0) {
    x = x0; y = y0;
    vx = vx0; vy = vy0;
    m = m0; r = r0;
    fx = 0; fy = 0;
}

// Actualiza posición: r(t+dt) = r(t) + v(t+dt/2) * dt 
// (Nota: se asume que 'vx' ya ha sido actualizado a medio paso antes de llamar esto)
void Bola::Mueva_r(double dt) {
    x += vx * dt;
    y += vy * dt;
}

// Actualiza velocidad: v(new) = v(old) + (F/m) * dt
void Bola::Mueva_v(double dt) {
    vx += (fx / m) * dt;
    vy += (fy / m) * dt;
}

void Bola::ResetFuerza() {
    fx = 0;
    fy = 0;
}

void Bola::AgregueFuerza(double Fx, double Fy) {
    fx += Fx;
    fy += Fy;
}

void Bola::ResuelvaColisionParedesRobusto(const Caja& C) {
    // Pared Izquierda
    if (x < r) {
        x = r;         // Corrección de posición
        vx *= -1.0;    // Rebote elástico
    }
    // Pared Derecha
    if (x > C.GetW() - r) {
        x = C.GetW() - r;
        vx *= -1.0;
    }
    // Pared Inferior
    if (y < r) {
        y = r;
        vy *= -1.0;
    }
    // Pared Superior
    if (y > C.GetH() - r) {
        y = C.GetH() - r;
        vy *= -1.0;
    }
}