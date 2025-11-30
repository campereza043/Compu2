/**
 * @file Sistema.h
 * @brief Define la clase Sistema, que representa el conjunto de bolas y la caja donde se realiza la simulación.
 * 
 * La clase Sistema coordina la evolución temporal de todas las bolas,
 * maneja la detección y resolución de colisiones, y gestiona la salida de datos.
 */

#ifndef SISTEMA_H
#define SISTEMA_H

#include "Caja.h"
#include "Bola.h"
#include <vector>
#include <fstream>
#include <string>

/**
 * @enum Integrador
 * @brief Enumeración para seleccionar el método de integración temporal.
 * 
 * Permite elegir entre los esquemas de Euler y Verlet.
 */
enum class Integrador { 
    Euler,  ///< Integración mediante el método de Euler explícito.
    Verlet  ///< Integración mediante el método de Verlet.
};

/**
 * @class Sistema
 * @brief Representa el sistema completo de simulación de un billar de N bolas.
 * 
 * Contiene la caja, las bolas y el método de integración seleccionado.
 * Se encarga de actualizar el sistema en cada paso de tiempo y guardar los resultados.
 */
class Sistema {
private:
    Caja caja;                    ///< Caja que define los límites del sistema.
    std::vector<Bola> bolas;      ///< Vector de bolas presentes en la simulación.
    Integrador integrador_actual = Integrador::Verlet; ///< Integrador usado en la simulación (por defecto: Verlet).

    /**
     * @brief Realiza un paso de integración usando el método de Euler.
     * @param dt Paso de tiempo.
     */
    void PasoEuler(double dt);

    /**
     * @brief Realiza un paso de integración usando el método de Verlet.
     * @param dt Paso de tiempo.
     */
    void PasoVerlet(double dt);

public:
    /**
     * @brief Define las dimensiones de la caja contenedora.
     * @param W Ancho de la caja.
     * @param H Alto de la caja.
     */
    void DefinaCaja(double W, double H);

    /**
     * @brief Reserva memoria para un número determinado de bolas.
     * @param N Número de bolas a reservar.
     */
    void Reserve(int N);

    /**
     * @brief Inicializa las bolas en una configuración de rejilla.
     * 
     * Las posiciones se distribuyen uniformemente y las velocidades
     * pueden asignarse aleatoriamente hasta un valor máximo dado.
     * 
     * @param m Masa de cada bola.
     * @param r Radio de cada bola.
     * @param v_max Velocidad máxima inicial.
     * @param alterna Si es verdadero, alterna el sentido de las velocidades.
     */
    void InicialiceRejilla(double m, double r, double v_max, bool alterna = false);

    /**
     * @brief Selecciona el integrador a utilizar.
     * 
     * @param nombre Nombre del integrador ("Euler" o "Verlet").
     */
    void SeleccioneIntegrador(const std::string& nombre);

    /**
     * @brief Ejecuta un paso temporal del sistema según el integrador actual.
     * @param dt Paso de tiempo.
     */
    void Paso(double dt);

    /**
     * @brief Escribe el encabezado de columnas en un archivo de salida.
     * @param f Flujo de salida (archivo abierto).
     */
    void Encabezado(std::ofstream& f);

    /**
     * @brief Guarda el estado actual del sistema en un archivo.
     * @param f Flujo de salida (archivo abierto).
     * @param t Tiempo actual de la simulación.
     */
    void Guarde(std::ofstream& f, double t);
};

#endif

