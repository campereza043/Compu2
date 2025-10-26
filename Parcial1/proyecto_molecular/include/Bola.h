#ifndef BOLA_H
#define BOLA_H

#include "Caja.h"
#include <cmath>

/**
 * @class Bola
 * @brief Representa una partícula esférica en la simulación
 * 
 * La clase Bola encapsula el estado físico de una partícula (posición, velocidad,
 * masa, radio) y proporciona métodos para su movimiento y colisiones.
 */
class Bola {
private:
    double x, y;    ///< Posición del centro
    double vx, vy;  ///< Componentes de velocidad
    double m;       ///< Masa de la partícula
    double r;       ///< Radio de la partícula

public:
    // Métodos de inicialización
    void Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0);
    
    // Métodos de movimiento
    void Muevase(double dt);
    void RebotePared(const Caja& caja);
    void ChoqueElastico(Bola& otra);
    
    // Getters
    double Getx() const { return x; }
    double Gety() const { return y; }
    double Getvx() const { return vx; }
    double Getvy() const { return vy; }
    double Getm() const { return m; }
    double Getr() const { return r; }
    
    // Setters para correcciones
    void Setx(double x_) { x = x_; }
    void Sety(double y_) { y = y_; }
};

#endif
