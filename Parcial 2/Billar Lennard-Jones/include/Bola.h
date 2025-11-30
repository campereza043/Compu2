/**
 * @file Bola.h
 * @brief Define la clase Bola que representa una partícula (bola) en el sistema de billar.
 * 
 * Esta clase contiene la posición, velocidad, masa y radio de una bola, 
 * así como los métodos necesarios para moverla y resolver colisiones con otras bolas o con las paredes.
 */

#ifndef BOLA_H
#define BOLA_H

#include "Caja.h"

/**
 * @class Bola
 * @brief Representa una bola en una simulación de billar 2D.
 * 
 * Cada bola posee propiedades físicas básicas (posición, velocidad, masa y radio)
 * y puede interactuar elásticamente con otras bolas y con las paredes de una caja.
 */
class Bola {
private:
    double x, y;   ///< Posición (x, y) de la bola en el plano.
    double vx, vy; ///< Componentes de la velocidad (vx, vy).
    double m;      ///< Masa de la bola.
    double r;      ///< Radio de la bola.

public:
    /** @brief Constructor por defecto. Inicializa una bola en el origen con masa y radio por defecto. */
    Bola();

    /**
     * @brief Inicializa los parámetros físicos de la bola.
     * @param x0 Posición inicial en x.
     * @param y0 Posición inicial en y.
     * @param vx0 Velocidad inicial en x.
     * @param vy0 Velocidad inicial en y.
     * @param m0 Masa.
     * @param r0 Radio.
     */
    void Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0);

    /**
     * @brief Actualiza la posición de la bola usando integración simple.
     * @param dt Paso de tiempo.
     */
    void Muevase(double dt);

    /**
     * @brief Resuelve colisiones simples con las paredes de la caja.
     * 
     * Invierte la velocidad al chocar con una pared, pero puede causar adherencia
     * si el tiempo de integración es grande.
     * 
     * @param C Caja con la que colisiona.
     */
    void ResuelvaColisionParedesSimple(const Caja& C);

    /**
     * @brief Resuelve colisiones robustas con las paredes de la caja.
     * 
     * Corrige tanto la posición como la velocidad para evitar que la bola atraviese las paredes.
     * 
     * @param C Caja con la que colisiona.
     */
    void ResuelvaColisionParedesRobusto(const Caja& C);

    /**
     * @brief Resuelve una colisión elástica con otra bola.
     * 
     * Conserva el momento lineal y la energía cinética en el sistema de dos bolas.
     * @param otra Referencia a la otra bola.
     */
    void ChoqueElastico(Bola& otra);

    // ===== Getters =====
    double Getx() const { return x; } ///< Retorna la coordenada x.
    double Gety() const { return y; } ///< Retorna la coordenada y.
    double Getvx() const { return vx; } ///< Retorna la componente vx.
    double Getvy() const { return vy; } ///< Retorna la componente vy.
    double Getm() const { return m; } ///< Retorna la masa.
    double Getr() const { return r; } ///< Retorna el radio.
};

#endif
