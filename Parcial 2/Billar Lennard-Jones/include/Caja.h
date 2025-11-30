/**
 * @file Caja.h
 * @brief Define la clase Caja que representa el contenedor rectangular del sistema de billar.
 * 
 * La clase Caja define las dimensiones del área de simulación (ancho y alto),
 * dentro de la cual se mueven las bolas y con cuyas paredes pueden colisionar.
 */

#ifndef CAJA_H
#define CAJA_H

/**
 * @class Caja
 * @brief Representa el recinto rectangular donde se mueven las bolas.
 * 
 * La caja define los límites espaciales del sistema de billar y se usa
 * para detectar y resolver colisiones con las paredes.
 */
class Caja {
private:
    double W; ///< Ancho de la caja.
    double H; ///< Alto de la caja.

public:
    /** @brief Constructor por defecto. Crea una caja sin dimensiones definidas. */
    Caja();

    /**
     * @brief Define las dimensiones de la caja.
     * @param W_ Ancho de la caja.
     * @param H_ Alto de la caja.
     */
    void Defina(double W_, double H_);

    /** @brief Retorna el ancho de la caja. */
    double GetW() const { return W; }

    /** @brief Retorna el alto de la caja. */
    double GetH() const { return H; }
};

#endif
