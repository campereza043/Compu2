#ifndef CAJA_H
#define CAJA_H

/**
 * @class Caja
 * @brief Representa la caja rectangular que contiene las partículas
 * 
 * La clase Caja define los límites espaciales donde se mueven las partículas
 * y proporciona métodos para manejar las colisiones con las paredes.
 */
class Caja {
private:
    double W; ///< Ancho de la caja
    double H; ///< Alto de la caja

public:
    /**
     * @brief Define las dimensiones de la caja
     * @param W_ Ancho de la caja (debe ser > 0)
     * @param H_ Alto de la caja (debe ser > 0)
     */
    void Defina(double W_, double H_);
    
    /**
     * @brief Obtiene el ancho de la caja
     * @return Ancho W
     */
    double GetW() const { return W; }
    
    /**
     * @brief Obtiene el alto de la caja
     * @return Alto H
     */
    double GetH() const { return H; }
    
    /**
     * @brief Verifica si una posición está dentro de la caja considerando el radio
     * @param x Coordenada x del centro
     * @param y Coordenada y del centro  
     * @param r Radio de la partícula
     * @return true si está dentro de los límites
     */
    bool EstaDentro(double x, double y, double r) const;
};

#endif
