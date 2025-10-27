/**
 * @file Sistema.cpp
 * @brief Implementación de la clase Sistema, que coordina la simulación del billar.
 * 
 * Este archivo contiene la lógica para el avance temporal, las colisiones entre bolas,
 * y la escritura de resultados a archivos de salida.
 */

#include "Sistema.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept> // std::invalid_argument

/**
 * @brief Selecciona el método de integración temporal.
 * 
 * @param nombre Nombre del integrador ("euler" o "verlet").
 * @throws std::invalid_argument Si el nombre no es válido.
 */
void Sistema::SeleccioneIntegrador(const std::string& nombre) {
    if (nombre == "euler") {
        integrador_actual = Integrador::Euler;
    } else if (nombre == "verlet") {
        integrador_actual = Integrador::Verlet;
    } else {
        throw std::invalid_argument("Integrador no válido. Elija 'euler' o 'verlet'.");
    }
}

/**
 * @brief Realiza un paso temporal del sistema según el integrador actual.
 * 
 * @param dt Paso de tiempo.
 */
void Sistema::Paso(double dt) {
    if (integrador_actual == Integrador::Euler)
        PasoEuler(dt);
    else
        PasoVerlet(dt);
}

/**
 * @brief Implementación del integrador de Euler.
 * 
 * Avanza la simulación mediante integración explícita.
 * Es sencillo, pero físicamente inexacto para colisiones rápidas.
 * 
 * @param dt Paso de tiempo.
 */
void Sistema::PasoEuler(double dt) {
    // 1. Mover todas las bolas
    for (auto& b : bolas)
        b.Muevase(dt);

    // 2. Resolver colisiones con paredes
    for (auto& b : bolas)
        b.ResuelvaColisionParedesSimple(caja);

    // 3. Resolver colisiones entre bolas
    for (size_t i = 0; i < bolas.size(); ++i)
        for (size_t j = i + 1; j < bolas.size(); ++j)
            bolas[i].ChoqueElastico(bolas[j]);
}

/**
 * @brief Implementación del integrador de Verlet.
 * 
 * Más estable y físicamente correcto, corrige la posición
 * de las bolas al colisionar con las paredes.
 * 
 * @param dt Paso de tiempo.
 */
void Sistema::PasoVerlet(double dt) {
    // 1. Mover todas las bolas
    for (auto& b : bolas)
        b.Muevase(dt);

    // 2. Resolver colisiones con paredes
    for (auto& b : bolas)
        b.ResuelvaColisionParedesRobusto(caja);

    // 3. Resolver colisiones entre bolas
    for (size_t i = 0; i < bolas.size(); ++i)
        for (size_t j = i + 1; j < bolas.size(); ++j)
            bolas[i].ChoqueElastico(bolas[j]);
}

/**
 * @brief Define las dimensiones de la caja de simulación.
 * @param W Ancho de la caja.
 * @param H Alto de la caja.
 */
void Sistema::DefinaCaja(double W, double H) {
    caja.Defina(W, H);
}

/**
 * @brief Reserva memoria para N bolas en la simulación.
 * @param N Número de bolas.
 */
void Sistema::Reserve(int N) {
    bolas.resize(N);
}

/**
 * @brief Inicializa las bolas en una distribución de rejilla.
 * 
 * Las bolas se ubican uniformemente dentro de la caja y se les asignan
 * velocidades aleatorias hasta un máximo `vmax`.
 * 
 * @param m Masa de cada bola.
 * @param r Radio de cada bola.
 * @param vmax Velocidad máxima inicial.
 * @param alterna Si es true, alterna la dirección de las velocidades.
 */
void Sistema::InicialiceRejilla(double m, double r, double vmax, bool alterna) {
    int N = bolas.size();
    if (N == 0) return;

    srand(time(nullptr));
    double W = caja.GetW(), H = caja.GetH();

    int cols = static_cast<int>(std::sqrt(N * W / H));
    int rows = (N > 0) ? (N + cols - 1) / cols : 0;

    double dx = (cols > 1) ? (W - 2*r) / (cols - 1) : W / 2.0;
    double dy = (rows > 1) ? (H - 2*r) / (rows - 1) : H / 2.0;

    for (int i = 0; i < N; ++i) {
        int row = i / cols;
        int col = i % cols;

        double x0 = (cols > 1) ? r + col * dx : W / 2.0;
        double y0 = (rows > 1) ? r + row * dy : H / 2.0;

        double ang = 2 * M_PI * ((double)rand() / RAND_MAX);
        double v = vmax * ((double)rand() / RAND_MAX);
        double vx = alterna && (i % 2 != 0) ? -v * cos(ang) : v * cos(ang);
        double vy = alterna && (i % 2 != 0) ? -v * sin(ang) : v * sin(ang);

        bolas[i].Inicie(x0, y0, vx, vy, m, r);
    }

    std::cout << "Inicialización en rejilla completada con " << N << " bolas.\n";
}

/**
 * @brief Escribe el encabezado de columnas en un archivo de salida.
 * @param f Archivo de salida abierto.
 */
void Sistema::Encabezado(std::ofstream& f) {
    f << "# " << std::setw(9) << "t";
    for (size_t i = 0; i < bolas.size(); i++) {
        f << std::setw(15) << "x" + std::to_string(i)
          << std::setw(15) << "y" + std::to_string(i)
          << std::setw(15) << "vx" + std::to_string(i)
          << std::setw(15) << "vy" + std::to_string(i);
    }
    f << "\n";
}

/**
 * @brief Guarda el estado actual del sistema en un archivo.
 * 
 * @param f Archivo de salida abierto.
 * @param t Tiempo actual de la simulación.
 */
void Sistema::Guarde(std::ofstream& f, double t) {
    f << std::setw(10) << std::fixed << std::setprecision(4) << t;
    for (const auto& b : bolas) {
        f << std::setw(15) << std::fixed << std::setprecision(6) << b.Getx()
          << std::setw(15) << std::fixed << std::setprecision(6) << b.Gety()
          << std::setw(15) << std::fixed << std::setprecision(6) << b.Getvx()
          << std::setw(15) << std::fixed << std::setprecision(6) << b.Getvy();
    }
    f << "\n";
}
