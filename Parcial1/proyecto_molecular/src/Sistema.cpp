#include "Sistema.h"
#include <cmath>
#include <random>
#include <stdexcept>
#include <iostream>

void Sistema::DefinaCaja(double W, double H) {
    caja.Defina(W, H);
}

void Sistema::Reserve(int N) {
    bolas.reserve(N);
}

bool Sistema::Colisionan(const Bola& a, const Bola& b) const {
    double dx = b.Getx() - a.Getx();
    double dy = b.Gety() - a.Gety();
    double distancia2 = dx*dx + dy*dy;
    double min_distancia = a.Getr() + b.Getr();
    return distancia2 <= (min_distancia * min_distancia);
}

void Sistema::InicialiceRejilla(double m, double r, double v0, bool alterna) {
    double W = caja.GetW();
    double H = caja.GetH();
    
    int N = bolas.capacity();
    
    // Calcular número de columnas y filas
    int n_cols = std::ceil(std::sqrt(N));
    int n_filas = (N + n_cols - 1) / n_cols; // División redondeada hacia arriba
    
    // Espaciado con márgenes
    double dx = (W - 2*r) / std::max(1, n_cols - 1);
    double dy = (H - 2*r) / std::max(1, n_filas - 1);
    
    // Asegurar que hay espacio suficiente
    if (dx < 2*r || dy < 2*r) {
        throw std::runtime_error("No hay espacio suficiente para la rejilla. Reduzca el número de partículas o el radio.");
    }
    
    // Inicializar semilla aleatoria
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_ang(0, 2*M_PI);
    
    int contador = 0;
    for (int i = 0; i < n_filas && contador < N; i++) {
        for (int j = 0; j < n_cols && contador < N; j++) {
            double x = r + j * dx;
            double y = r + i * dy;
            
            // Velocidad inicial
            double angulo;
            if (alterna) {
                angulo = ((i + j) % 2 == 0) ? 0 : M_PI;
            } else {
                angulo = dis_ang(gen);
            }
            
            double vx = v0 * std::cos(angulo);
            double vy = v0 * std::sin(angulo);
            
            Bola nueva_bola;
            nueva_bola.Inicie(x, y, vx, vy, m, r);
            bolas.push_back(nueva_bola);
            contador++;
        }
    }
}

bool Sistema::InicialiceAleatorio(double m, double r, double vmax, int max_intentos) {
    double W = caja.GetW();
    double H = caja.GetH();
    int N = bolas.capacity();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis_x(r, W - r);
    std::uniform_real_distribution<double> dis_y(r, H - r);
    std::uniform_real_distribution<double> dis_ang(0, 2*M_PI);
    std::uniform_real_distribution<double> dis_v(0, vmax);
    
    for (int i = 0; i < N; i++) {
        bool posicion_valida = false;
        int intentos = 0;
        
        while (!posicion_valida && intentos < max_intentos) {
            double x = dis_x(gen);
            double y = dis_y(gen);
            
            // Verificar solapamiento con partículas existentes
            posicion_valida = true;
            for (const auto& bola : bolas) {
                double dx = x - bola.Getx();
                double dy = y - bola.Gety();
                double distancia2 = dx*dx + dy*dy;
                double min_distancia = r + bola.Getr();
                
                if (distancia2 < min_distancia * min_distancia) {
                    posicion_valida = false;
                    break;
                }
            }
            
            if (posicion_valida) {
                double angulo = dis_ang(gen);
                double v = dis_v(gen);
                double vx = v * std::cos(angulo);
                double vy = v * std::sin(angulo);
                
                Bola nueva_bola;
                nueva_bola.Inicie(x, y, vx, vy, m, r);
                bolas.push_back(nueva_bola);
                break;
            }
            
            intentos++;
        }
        
        if (!posicion_valida) {
            std::cerr << "Error: No se pudo colocar la partícula " << i + 1 
                      << " después de " << max_intentos << " intentos" << std::endl;
            return false;
        }
    }
    
    return true;
}

void Sistema::Paso(double dt) {
    // 1. Mover todas las partículas
    for (auto& bola : bolas) {
        bola.Muevase(dt);
    }
    
    // 2. Rebotar con paredes
    for (auto& bola : bolas) {
        bola.RebotePared(caja);
    }
    
    // 3. Detectar y resolver colisiones entre partículas (O(N^2))
    int N = bolas.size();
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (Colisionan(bolas[i], bolas[j])) {
                bolas[i].ChoqueElastico(bolas[j]);
            }
        }
    }
    
    tiempo_actual += dt;
    pasos++;
}

void Sistema::Encabezado(std::ofstream& f) const {
    f << "# t";
    for (int i = 0; i < bolas.size(); i++) {
        f << " x" << i << " y" << i << " vx" << i << " vy" << i;
    }
    f << "\n";

void Sistema::Guarde(std::ofstream& f, double t) const {
    f << std::scientific << std::setprecision(6) << t;
    for (const auto& bola : bolas) {
        f << " " << bola.Getx()
          << " " << bola.Gety()
          << " " << bola.Getvx()
          << " " << bola.Getvy();
    }
    f << "\n";
}
