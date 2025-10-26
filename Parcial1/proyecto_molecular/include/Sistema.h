#ifndef SISTEMA_H
#define SISTEMA_H

#include "Bola.h"
#include <vector>
#include <fstream>
#include <iomanip>

/**
 * @class Sistema  
 * @brief Gestiona todo el sistema de partículas y la simulación
 *
 * La clase Sistema coordina la caja, el conjunto de partículas y la evolución
 * temporal del sistema, incluyendo detección de colisiones y salida de datos.
 */
class Sistema {
private:
    Caja caja;                   ///< Caja que contiene las partículas
    std::vector<Bola> bolas;     ///< Vector de partículas
    double tiempo_actual;        ///< Tiempo actual de simulación
    int pasos;                   ///< Contador de pasos de simulación

    /**
     * @brief Verifica si dos partículas están colisionando
     * @param a Primera partícula
     * @param b Segunda partícula  
     * @return true si hay colisión
     */
    bool Colisionan(const Bola& a, const Bola& b) const;

public:
    // Configuración
    void DefinaCaja(double W, double H);
    void Reserve(int N);
    
    // Inicialización
    void InicialiceRejilla(double m, double r, double v0, bool alterna = false);
    bool InicialiceAleatorio(double m, double r, double vmax, int max_intentos = 1000);
    
    // Simulación
    void Paso(double dt);
    
    // Salida de datos
    void Encabezado(std::ofstream& f) const;
    void Guarde(std::ofstream& f, double t) const;
    
    // Getters para análisis
    int GetNumBolas() const { return bolas.size(); }
    const Bola& GetBola(int i) const { return bolas[i]; }
};

#endif
