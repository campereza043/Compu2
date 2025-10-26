#ifndef BOLA_H
#define BOLA_H

#include "Caja.h"

/**
 * @class Bola
 * @brief Modela una partícula esférica.
 *
 * Responsabilidad: Encapsular los atributos físicos (posición, velocidad,
 * masa, radio) y gestionar su movimiento y colisiones.
 */
class Bola {
private:
  double x, y;   ///< Posición (x, y)
  double vx, vy; ///< Velocidad (vx, vy)
  double m, r;   ///< Masa (m) y radio (r)

public:
  /**
   * @brief Inicializa los atributos de la bola.
   */
  void Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0);

  /**
   * @brief Actualiza la posición de la bola usando la integración de Euler.
   * @param dt Paso de tiempo.
   */
  void Muevase(double dt);

  /**
   * @brief Gestiona el choque elástico con las paredes de la caja.
   * @param C Referencia constante al objeto Caja.
   */
  void RebotePared(const Caja &C);

  /**
   * @brief Gestiona el choque elástico con otra bola.
   * @param otra Referencia a otro objeto Bola.
   */
  void ChoqueElastico(Bola &otra);

  // Getters para acceder a los atributos privados
  double Getx() const { return x; }
  double Gety() const { return y; }
  double Getvx() const { return vx; }
  double Getvy() const { return vy; }
  double Getm() const { return m; }
  double Getr() const { return r; }
};

#endif // BOLA_H
