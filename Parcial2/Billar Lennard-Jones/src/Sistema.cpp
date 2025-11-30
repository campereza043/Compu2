/**
 * @file Sistema.cpp
 * @brief Implementación de Dinámica Molecular (LJ + Velocity Verlet).
 */
#define _USE_MATH_DEFINES
#include "Sistema.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>

Sistema::Sistema() : epsilon(1.0), sigma(1.0) {}

void Sistema::DefinaCaja(double W, double H) {
    caja.Defina(W, H);
}

void Sistema::DefinaParametrosLJ(double eps, double sig) {
    epsilon = eps;
    sigma = sig;
}

void Sistema::Reserve(int N) {
    bolas.resize(N);
}

void Sistema::InicialiceRejilla(double m, double r, double vmax) {
    int N = bolas.size();
    if (N == 0) return;
    srand(1); // Semilla fija para reproducibilidad

    double W = caja.GetW();
    double H = caja.GetH();
    
    int cols = static_cast<int>(std::sqrt(N * W / H));
    int rows = (N + cols - 1) / cols; 
    
    // Distanciamiento para evitar explosión inicial por LJ
    double dx = W / (cols + 1);
    double dy = H / (rows + 1);

    for (int i = 0; i < N; ++i) {
        int row = i / cols;
        int col = i % cols;
        
        double x0 = (col + 1) * dx;
        double y0 = (row + 1) * dy;
        
        // Velocidades aleatorias
        double theta = 2.0 * M_PI * ((double)rand() / RAND_MAX);
        double v = vmax * ((double)rand() / RAND_MAX);
        
        bolas[i].Inicie(x0, y0, v*cos(theta), v*sin(theta), m, r);
    }
    // Calcular fuerzas iniciales para el primer paso de Verlet
    CalculeFuerzas();
}

void Sistema::CalculeFuerzas() {
    // 1. Resetear fuerzas de todas las partículas
    for (auto& b : bolas) b.ResetFuerza();

    // 2. Interacción Lennard-Jones (Pares) [cite: 24, 25]
    int N = bolas.size();
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double dx = bolas[j].Getx() - bolas[i].Getx();
            double dy = bolas[j].Gety() - bolas[i].Gety();
            double r2 = dx*dx + dy*dy;
            
            // Evitar singularidad numérica y optimización de cutoff (opcional)
            if (r2 < 1e-4) r2 = 1e-4; 
            
            double r = std::sqrt(r2);
            
            // Cálculo optimizado de LJ
            // F_mag = (24*eps/r) * [ 2*(sigma/r)^12 - (sigma/r)^6 ]
            // F_vec = F_mag * (vec_r / r) = (F_mag / r) * vec_r
            
            double s_r = sigma / r;
            double s_r_6 = s_r * s_r * s_r * s_r * s_r * s_r; // (sigma/r)^6
            double s_r_12 = s_r_6 * s_r_6;                    // (sigma/r)^12
            
            // Magnitud de la fuerza escalar dividida por r (para vectorizar)
            double F_over_r = (24.0 * epsilon / r2) * (2.0 * s_r_12 - s_r_6);
            
            double Fx = F_over_r * dx;
            double Fy = F_over_r * dy;

            // Tercera ley de Newton: F_ji = -F_ij
            bolas[i].AgregueFuerza(-Fx, -Fy); // Atrae o repele a i
            bolas[j].AgregueFuerza(Fx, Fy);   // Atrae o repele a j
        }
    }
}

// Implementación de Velocity-Verlet 
void Sistema::PasoVelocityVerlet(double dt) {
    // 1. Primer medio paso de velocidad: v(t + dt/2) = v(t) + 0.5*a(t)*dt
    for (auto& b : bolas) {
        b.Mueva_v(0.5 * dt); 
    }

    // 2. Paso completo de posición: r(t + dt) = r(t) + v(t + dt/2)*dt
    for (auto& b : bolas) {
        b.Mueva_r(dt);
        // Verificar condiciones de frontera inmediatamente al mover
        b.ResuelvaColisionParedesRobusto(caja); 
    }

    // 3. Calcular fuerzas nuevas: a(t + dt)
    CalculeFuerzas();

    // 4. Segundo medio paso de velocidad: v(t + dt) = v(t + dt/2) + 0.5*a(t + dt)*dt
    for (auto& b : bolas) {
        b.Mueva_v(0.5 * dt);
    }
}

void Sistema::Paso(double dt) {
    PasoVelocityVerlet(dt);
}

void Sistema::Encabezado(std::ofstream& f) {
    f << "t";
    for(size_t i=0; i<bolas.size(); ++i) 
        f << ",x" << i << ",y" << i << ",vx" << i << ",vy" << i;
    f << "\n";
}

void Sistema::Guarde(std::ofstream& f, double t) {
    f << t;
    for (const auto& b : bolas) {
        f << "," << b.Getx() << "," << b.Gety() 
          << "," << b.Getvx() << "," << b.Getvy();
    }
    f << "\n";
}