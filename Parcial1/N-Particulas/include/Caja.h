#ifndef CAJA_H
#define CAJA_H

/**
 * @class Caja
 * @brief Modela el dominio rectangular donde se mueven las partículas.
 *
 * Responsabilidad: Delimitar el dominio y gestionar las dimensiones
 * para los rebotes contra las paredes.
 */
class Caja {
public:
  double W, H; ///< Ancho (W) y alto (H) de la caja.

  /**
   * @brief Constructor por defecto.
   */
  Caja() : W(0), H(0) {}

  /**
   * @brief Define las dimensiones de la caja.
   * @param W_ Ancho de la caja.
   * @param H_ Alto de la caja.
   */
  void Defina(double W_, double H_);
};

#endif // CAJA_H
