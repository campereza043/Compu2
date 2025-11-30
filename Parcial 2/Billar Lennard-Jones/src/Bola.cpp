/**
 * @file Bola.cpp
 * @brief Implementación de los métodos de la clase Bola.
 * 
 * Contiene la lógica para el movimiento de las bolas, 
 * las colisiones con las paredes y los choques elásticos entre bolas.
 */

#include "Bola.h"
#include <cmath>

/**
 * @brief Constructor por defecto.
 * 
 * Inicializa la bola en el origen con velocidad cero, 
 * masa unitaria y radio 0.1.
 */
Bola::Bola() : x(0), y(0), vx(0), vy(0), m(1.0), r(0.1) {}

/**
 * @brief Inicializa los parámetros físicos de la bola.
 * 
 * @param x0 Posición inicial en x.
 * @param y0 Posición inicial en y.
 * @param vx0 Velocidad inicial en x.
 * @param vy0 Velocidad inicial en y.
 * @param m0 Masa.
 * @param r0 Radio.
 */
void Bola::Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0) {
    x = x0; 
    y = y0;
    vx = vx0; 
    vy = vy0;
    m = m0; 
    r = r0;
}

/**
 * @brief Avanza la posición de la bola según su velocidad.
 * 
 * Este método no considera colisiones; simplemente actualiza
 * la posición usando el paso de tiempo `dt`.
 * 
 * @param dt Paso de tiempo.
 */
void Bola::Muevase(double dt) {
    x += vx * dt;
    y += vy * dt;
}

/**
 * @brief Resuelve colisiones simples con las paredes de la caja.
 * 
 * Si la bola toca una pared y se mueve hacia ella, su velocidad
 * se invierte. Este método no corrige la posición y puede causar
 * “pegado” si el paso de tiempo es grande.
 * 
 * @param C Caja con la que colisiona.
 */
void Bola::ResuelvaColisionParedesSimple(const Caja& C) {
    if (x - r < 0 && vx < 0) vx *= -1;
    if (x + r > C.GetW() && vx > 0) vx *= -1;
    if (y - r < 0 && vy < 0) vy *= -1;
    if (y + r > C.GetH() && vy > 0) vy *= -1;
}

/**
 * @brief Resuelve colisiones robustas con las paredes de la caja.
 * 
 * Además de invertir la velocidad, este método ajusta la posición 
 * de la bola para evitar que atraviese las paredes, 
 * mejorando la estabilidad numérica de la simulación.
 * 
 * @param C Caja con la que colisiona.
 */
void Bola::ResuelvaColisionParedesRobusto(const Caja& C) {
    if (x - r < 0 && vx < 0) {
        x = r + (r - x); ///< Corrige posición en x.
        vx *= -1;
    }
    if (x + r > C.GetW() && vx > 0) {
        x = C.GetW() - r - (x + r - C.GetW());
        vx *= -1;
    }
    if (y - r < 0 && vy < 0) {
        y = r + (r - y);
        vy *= -1;
    }
    if (y + r > C.GetH() && vy > 0) {
        y = C.GetH() - r - (y + r - C.GetH());
        vy *= -1;
    }
}

/**
 * @brief Resuelve una colisión elástica entre dos bolas.
 * 
 * Conserva el momento lineal y la energía cinética del sistema 
 * de dos bolas. Si las bolas se solapan, también realiza una 
 * corrección de posición para separarlas.
 * 
 * @param otra Referencia a la otra bola con la que colisiona.
 */
void Bola::ChoqueElastico(Bola& otra) {
    double dx = otra.x - x;
    double dy = otra.y - y;
    double dist_sq = dx * dx + dy * dy;
    double minDist = r + otra.r;

    // Detecta superposición
    if (dist_sq < minDist * minDist) {
        double dist = std::sqrt(dist_sq);
        // Evita división por cero si están en el mismo punto
        if (dist == 0.0) return;

        // Vector unitario normal
        double nx = dx / dist;
        double ny = dy / dist;

        // Diferencia de velocidades
        double dvx = otra.vx - vx;
        double dvy = otra.vy - vy;
        double vn = dvx * nx + dvy * ny;

        // Solo aplica la colisión si se acercan entre sí
        if (vn < 0) {
            double J = (-2 * vn) / (1/m + 1/otra.m); ///< Impulso escalar.
            vx -= (J / m) * nx;
            vy -= (J / m) * ny;
            otra.vx += (J / otra.m) * nx;
            otra.vy += (J / otra.m) * ny;
        }

        // Corrección por superposición (ligero desplazamiento)
        double overlap = 0.51 * (minDist - dist);
        x -= overlap * nx;
        y -= overlap * ny;
        otra.x += overlap * nx;
        otra.y += overlap * ny;
    }
}
