/**
 * @file Caja.cpp
 * @brief Implementación de la clase Caja.
 * 
 * Contiene la definición de los métodos que inicializan y configuran
 * las dimensiones del recinto donde se mueven las bolas.
 */

#include "Caja.h"

/**
 * @brief Constructor por defecto.
 * 
 * Inicializa una caja de dimensiones 1x1.
 */
Caja::Caja() : W(1.0), H(1.0) {}

/**
 * @brief Define las dimensiones de la caja.
 * 
 * @param W_ Ancho de la caja.
 * @param H_ Alto de la caja.
 */
void Caja::Defina(double W_, double H_) {
    W = W_;
    H = H_;
}
