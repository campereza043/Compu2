#include "Sistema.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept> // For std::invalid_argument

void Sistema::SeleccioneIntegrador(const std::string& nombre) {
    if (nombre == "euler") {
        integrador_actual = Integrador::Euler;
    } else if (nombre == "verlet") {
        integrador_actual = Integrador::Verlet;
    } else {
        throw std::invalid_argument("Integrador no valido. Elija 'euler' o 'verlet'.");
    }
}

void Sistema::Paso(double dt) {
    // Dispatch to the correct integration method
    if (integrador_actual == Integrador::Euler) {
        PasoEuler(dt);
    } else {
        PasoVerlet(dt);
    }
}

// --- EULER METHOD (PROTOTYPE) ---
// Simple but physically inaccurate wall collisions.
void Sistema::PasoEuler(double dt) {
    // 1. Move all particles (Euler step)
    for (auto& b : bolas) {
        b.Muevase(dt);
    }

    // 2. Handle collisions
    // Walls (simple, inaccurate reflection)
    for (auto& b : bolas) {
        b.ResuelvaColisionParedesSimple(caja);
    }
    // Between particles
    for (size_t i = 0; i < bolas.size(); ++i) {
        for (size_t j = i + 1; j < bolas.size(); ++j) {
            bolas[i].ChoqueElastico(bolas[j]);
        }
    }
}

// --- VELOCITY-VERLET METHOD (STABLE & RECOMMENDED) ---
// More robust wall collision handling that prevents sticking.
void Sistema::PasoVerlet(double dt) {
    // 1. Move all particles (same as Euler for F=0)
    for (auto& b : bolas) {
        b.Muevase(dt);
    }

    // 2. Handle collisions
    // Walls (robust, position-correcting reflection)
    for (auto& b : bolas) {
        b.ResuelvaColisionParedesRobusto(caja);
    }
    // Between particles
    for (size_t i = 0; i < bolas.size(); ++i) {
        for (size_t j = i + 1; j < bolas.size(); ++j) {
            bolas[i].ChoqueElastico(bolas[j]);
        }
    }
}

// --- Other System Methods (Setup and I/O) ---

void Sistema::DefinaCaja(double W, double H) {
    caja.Defina(W, H);
}

void Sistema::Reserve(int N) {
    bolas.resize(N);
}

void Sistema::InicialiceRejilla(double m, double r, double vmax, bool alterna) {
    int N = bolas.size();
    if (N == 0) return;

    srand(time(nullptr));
    double W = caja.GetW();
    double H = caja.GetH();

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
    std::cout << "Inicializacion en rejilla completada con " << N << " bolas.\n";
}

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
