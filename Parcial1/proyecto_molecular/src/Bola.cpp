#include "Bola.h"
#include <stdexcept>
#include <iostream>
#include <cmath>

void Bola::Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0) {
    if (m0 <= 0 || r0 <= 0) {
        throw std::invalid_argument("Masa y radio deben ser positivos");
    }
    x = x0;
    y = y0;
    vx = vx0;
    vy = vy0;
    m = m0;
    r = r0;
}

void Bola::Muevase(double dt) {
    // Integración Euler simple
    x += vx * dt;
    y += vy * dt;
}

void Bola::RebotePared(const Caja& caja) {
    double W = caja.GetW();
    double H = caja.GetH();
    
    // Pared izquierda - con corrección para evitar valores negativos
    if (x - r < 0) {
        vx = std::abs(vx); // Siempre hacia la derecha
        x = r;
    }
    // Pared derecha
    if (x + r > W) {
        vx = -std::abs(vx); // Siempre hacia la izquierda
        x = W - r;
    }
    // Pared inferior
    if (y - r < 0) {
        vy = std::abs(vy); // Siempre hacia arriba
        y = r;
    }
    // Pared superior
    if (y + r > H) {
        vy = -std::abs(vy); // Siempre hacia abajo
        y = H - r;
    }
}

void Bola::ChoqueElastico(Bola& otra) {
    // Vector normal (de esta a otra)
    double dx = otra.x - x;
    double dy = otra.y - y;
    double distancia2 = dx*dx + dy*dy;
    
    // Evitar división por cero
    if (distancia2 < 1e-20) {
        // Si están demasiado cerca, separarlas aleatoriamente
        dx = 1e-10;
        dy = 0;
        distancia2 = 1e-20;
    }
    
    double distancia = std::sqrt(distancia2);
    
    // Normalizar
    double nx = dx / distancia;
    double ny = dy / distancia;
    
    // Velocidades en dirección normal
    double v1n = vx * nx + vy * ny;
    double v2n = otra.vx * nx + otra.vy * ny;
    
    // Conservación de momento y energía (masas iguales)
    // Para colisión elástica con masas iguales, las velocidades se intercambian
    double nuevo_v1n = v2n;
    double nuevo_v2n = v1n;
    
    // Actualizar velocidades
    vx += (nuevo_v1n - v1n) * nx;
    vy += (nuevo_v1n - v1n) * ny;
    
    otra.vx += (nuevo_v2n - v2n) * nx;
    otra.vy += (nuevo_v2n - v2n) * ny;
    
    // Separar partículas para evitar solapamiento persistente
    double solapamiento = (r + otra.r) - distancia;
    if (solapamiento > 0) {
        double correccion = solapamiento * 0.51; // Factor ligeramente > 0.5 para asegurar separación
        x -= correccion * nx;
        y -= correccion * ny;
        otra.x += correccion * nx;
        otra.y += correccion * ny;
    }
}
