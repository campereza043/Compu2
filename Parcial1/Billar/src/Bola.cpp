#include "Bola.h"
#include <cmath>

Bola::Bola() : x(0), y(0), vx(0), vy(0), m(1.0), r(0.1) {}

void Bola::Inicie(double x0, double y0, double vx0, double vy0, double m0, double r0) {
    x = x0; y = y0;
    vx = vx0; vy = vy0;
    m = m0; r = r0;
}

// Just advances position based on velocity. Collisions handled by Sistema.
void Bola::Muevase(double dt) {
    x += vx * dt;
    y += vy * dt;
}

// Simple collision: just inverts velocity. Can cause sticking.
void Bola::ResuelvaColisionParedesSimple(const Caja& C) {
    if (x - r < 0 && vx < 0) vx *= -1;
    if (x + r > C.GetW() && vx > 0) vx *= -1;
    if (y - r < 0 && vy < 0) vy *= -1;
    if (y + r > C.GetH() && vy > 0) vy *= -1;
}

// Robust collision: corrects position to avoid passing through the wall.
void Bola::ResuelvaColisionParedesRobusto(const Caja& C) {
    if (x - r < 0 && vx < 0) {
        x = r + (r - x); // Reflect position
        vx *= -1;
    }
    if (x + r > C.GetW() && vx > 0) {
        x = C.GetW() - r - (x + r - C.GetW()); // Reflect position
        vx *= -1;
    }
    if (y - r < 0 && vy < 0) {
        y = r + (r - y); // Reflect position
        vy *= -1;
    }
    if (y + r > C.GetH() && vy > 0) {
        y = C.GetH() - r - (y + r - C.GetH()); // Reflect position
        vy *= -1;
    }
}

// Elastic collision between two balls (unchanged from your original)
void Bola::ChoqueElastico(Bola& otra) {
    double dx = otra.x - x;
    double dy = otra.y - y;
    double dist_sq = dx * dx + dy * dy;
    double minDist = r + otra.r;

    if (dist_sq < minDist * minDist) {
        double dist = std::sqrt(dist_sq);
        // Evita división por cero si están exactamente en el mismo punto
        if (dist == 0.0) return;

        double nx = dx / dist;
        double ny = dy / dist;

        double dvx = otra.vx - vx;
        double dvy = otra.vy - vy;
        double vn = dvx * nx + dvy * ny;

        // Only apply collision if they are moving towards each other
        if (vn < 0) {
            double J = (-2 * vn) / (1/m + 1/otra.m);
            vx -= (J / m) * nx;
            vy -= (J / m) * ny;
            otra.vx += (J / otra.m) * nx;
            otra.vy += (J / otra.m) * ny;
        }

        // Overlap correction
        double overlap = 0.51 * (minDist - dist);
        x -= overlap * nx;
        y -= overlap * ny;
        otra.x += overlap * nx;
        otra.y += overlap * ny;
    }
}
