#include "Bola.h"
#include <cmath>

void Bola::Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0) {
  x = x0; y = y0;
  vx = vx0; vy = vy0;
  m = m0; r = r0;
}

void Bola::Muevase(double dt) {
  x += vx * dt;
  y += vy * dt;
}

void Bola::RebotePared(const Caja &C) {
  // Choque con pared derecha
  if (x > C.W - r) {
    vx = -std::abs(vx);
    x = C.W - r;
  }
  // Choque con pared izquierda
  if (x < r) {
    vx = std::abs(vx);
    x = r;
  }
  // Choque con pared superior
  if (y > C.H - r) {
    vy = -std::abs(vy);
    y = C.H - r;
  }
  // Choque con pared inferior
  if (y < r) {
    vy = std::abs(vy);
    y = r;
  }
}

void Bola::ChoqueElastico(Bola &otra) {
  double dx = otra.x - x;
  double dy = otra.y - y;
  double d2 = dx * dx + dy * dy;
  double r_sum = r + otra.r;

  // Condición de colisión: distancia < suma de radios
  if (d2 < r_sum * r_sum) {
    double d = std::sqrt(d2);
    // Vector normal de colisión (unitario)
    double nx = dx / d;
    double ny = dy / d;

    // Velocidades relativas
    double dvx = otra.vx - vx;
    double dvy = otra.vy - vy;

    // Proyección de la velocidad relativa en el vector normal
    double v_rel_normal = dvx * nx + dvy * ny;

    // Si las partículas se están acercando
    if (v_rel_normal < 0) {
      double J = -2 * v_rel_normal / (1 / m + 1 / otra.m);
      
      // Impulso aplicado
      double Jx = J * nx;
      double Jy = J * ny;

      // Actualizar velocidades
      vx -= Jx / m;
      vy -= Jy / m;
      otra.vx += Jx / otra.m;
      otra.vy += Jy / otra.m;
    }
  }
}
