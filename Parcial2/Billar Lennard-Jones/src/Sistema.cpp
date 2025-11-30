/**
 * @file Sistema.cpp
 * @brief Implementación de Dinámica Molecular (Salida formato .dat con tabs).
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
    srand(1); 

    double W = caja.GetW();
    double H = caja.GetH();
    
    int cols = static_cast<int>(std::sqrt(N * W / H));
    int rows = (N + cols - 1) / cols; 
    
    double dx = W / (cols + 1);
    double dy = H / (rows + 1);

    for (int i = 0; i < N; ++i) {
        int row = i / cols;
        int col = i % cols;
        
        double x0 = (col + 1) * dx;
        double y0 = (row + 1) * dy;
        
        double theta = 2.0 * M_PI * ((double)rand() / RAND_MAX);
        double v = vmax * ((double)rand() / RAND_MAX);
        
        bolas[i].Inicie(x0, y0, v*cos(theta), v*sin(theta), m, r);
    }
    CalculeFuerzas();
}

void Sistema::CalculeFuerzas() {
    for (auto& b : bolas) b.ResetFuerza();

    int N = bolas.size();
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double dx = bolas[j].Getx() - bolas[i].Getx();
            double dy = bolas[j].Gety() - bolas[i].Gety();
            double r2 = dx*dx + dy*dy;
            
            if (r2 < 1e-4) r2 = 1e-4; 
            
            double r = std::sqrt(r2);
            double s_r = sigma / r;
            double s_r_6 = s_r * s_r * s_r * s_r * s_r * s_r; 
            double s_r_12 = s_r_6 * s_r_6;                    
            
            double F_over_r = (24.0 * epsilon / r2) * (2.0 * s_r_12 - s_r_6);
            
            double Fx = F_over_r * dx;
            double Fy = F_over_r * dy;

            bolas[i].AgregueFuerza(-Fx, -Fy); 
            bolas[j].AgregueFuerza(Fx, Fy);   
        }
    }
}

void Sistema::PasoVelocityVerlet(double dt) {
    for (auto& b : bolas) {
        b.Mueva_v(0.5 * dt); 
    }

    for (auto& b : bolas) {
        b.Mueva_r(dt);
        b.ResuelvaColisionParedesRobusto(caja); 
    }

    CalculeFuerzas();

    for (auto& b : bolas) {
        b.Mueva_v(0.5 * dt);
    }
}

void Sistema::Paso(double dt) {
    PasoVelocityVerlet(dt);
}

// MODIFICADO: Uso de \t para separar columnas (.dat style)
void Sistema::Encabezado(std::ofstream& f) {
    f << "t";
    for(size_t i=0; i<bolas.size(); ++i) 
        f << "\tx" << i << "\ty" << i << "\tvx" << i << "\tvy" << i;
    f << "\n";
}

// MODIFICADO: Uso de \t para separar datos
void Sistema::Guarde(std::ofstream& f, double t) {
    f << t;
    for (const auto& b : bolas) {
        f << "\t" << b.Getx() << "\t" << b.Gety() 
          << "\t" << b.Getvx() << "\t" << b.Getvy();
    }
    f << "\n";
}