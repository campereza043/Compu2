#include "Sistema.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <ctime>

void Sistema::DefinaCaja(double W, double H) {
  caja.Defina(W, H);
}

void Sistema::Reserve(int N) {
  bolas.resize(N);
}

void Sistema::InicialiceRejilla(double m, double r, double vmax) {
  srand(time(NULL)); // Semilla para números aleatorios

  int N = bolas.size();
  int cols = static_cast<int>(std::sqrt(N * caja.W / caja.H));
  int rows = (N + cols - 1) / cols;

  double dx = caja.W / cols;
  double dy = caja.H / rows;

  for (int i = 0; i < N; ++i) {
    int row = i / cols;
    int col = i % cols;

    double x0 = (col + 0.5) * dx;
    double y0 = (row + 0.5) * dy;

    // Velocidad aleatoria
    double angle = (double)rand() / RAND_MAX * 2 * M_PI;
    double v_mag = (double)rand() / RAND_MAX * vmax;
    double vx0 = v_mag * cos(angle);
    double vy0 = v_mag * sin(angle);

    bolas[i].Inicie(x0, y0, vx0, vy0, m, r);
  }
}

void Sistema::Paso(double dt) {
  // 1. Mover todas las bolas
  for (auto &bola : bolas) {
    bola.Muevase(dt);
  }

  // 2. Revisar rebotes con las paredes
  for (auto &bola : bolas) {
    bola.RebotePared(caja);
  }

  // 3. Revisar colisiones entre pares de bolas
  for (size_t i = 0; i < bolas.size(); ++i) {
    for (size_t j = i + 1; j < bolas.size(); ++j) {
      bolas[i].ChoqueElastico(bolas[j]);
    }
  }
}

void Sistema::Encabezado(std::ofstream &f) {
  f << std::left;
  f << std::setw(15) << "t";
  for (size_t i = 0; i < bolas.size(); ++i) {
    f << std::setw(15) << ("x" + std::to_string(i))
      << std::setw(15) << ("y" + std::to_string(i))
      << std::setw(15) << ("vx" + std::to_string(i))
      << std::setw(15) << ("vy" + std::to_string(i));
  }
  f << std::endl;
}

void Sistema::Guarde(std::ofstream &f, double t) {
  f << std::scientific << std::setprecision(6) << std::left;
  f << std::setw(15) << t;
  for (const auto &bola : bolas) {
    f << std::setw(15) << bola.Getx()
      << std::setw(15) << bola.Gety()
      << std::setw(15) << bola.Getvx()
      << std::setw(15) << bola.Getvy();
  }
  f << std::endl;
}
