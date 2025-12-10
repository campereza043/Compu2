#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>

#include "../include/AsymmetricPotential.h"
#include "../include/FlashingMotor.h"
#include "../include/RockingMotor.h"
#include "../include/CoupledMotors.h"

void solicitar_datos(double &kc, double &d0,
                     double &T_on, double &T_off,
                     double &A, double &omega,
                     double &total_time, double &dt,
                     double &gamma, double &kBT)
{
    std::cout << "=== Configuración del Sistema Acoplado (Flashing + Rocking) ===\n";

    std::cout << "Constante del resorte (kc) [Sugerido: 5.0]: ";
    std::cin  >> kc;

    std::cout << "Distancia natural del resorte (d0) [Sugerido: 0.0]: ";
    std::cin  >> d0;

    std::cout << "--- Parámetros del Motor Flashing ---\n";
    std::cout << "Tiempo encendido (T_on) [Sugerido: 50.0]: ";
    std::cin  >> T_on;

    std::cout << "Tiempo apagado (T_off) [Sugerido: 20.0]: ";
    std::cin  >> T_off;

    std::cout << "--- Parámetros del Motor Rocking ---\n";
    std::cout << "Amplitud driving (A) [Sugerido: 0.5]: ";
    std::cin  >> A;

    std::cout << "Frecuencia (omega) [Sugerido: 1.0]: ";
    std::cin  >> omega;

    std::cout << "--- Parámetros Numéricos Globales ---\n";
    std::cout << "Tiempo total de simulación [Sugerido: 200.0]: ";
    std::cin  >> total_time;

    std::cout << "Paso temporal dt [Sugerido: 0.001]: ";
    std::cin  >> dt;

    std::cout << "--- Parámetros Termodinámicos (Langevin) ---\n";
    std::cout << "Coeficiente de fricción gamma [Sugerido: 0.5]: ";
    std::cin  >> gamma;

    std::cout << "Temperatura kBT [Sugerido: 0.8]: ";
    std::cin  >> kBT;

    std::cout << "=== Iniciando simulación... ===\n\n";
}

int main(int argc, char** argv) {

    double kc, d0, T_on, T_off, A, omega, total_time, dt, gamma, kBT;

    // Si el usuario NO pasa argumentos → modo interactivo
    if (argc != 11) {
        solicitar_datos(kc, d0, T_on, T_off, A, omega, total_time, dt, gamma, kBT);
    } else {
        // Modo batch (opcional)
        kc         = atof(argv[1]);
        d0         = atof(argv[2]);
        T_on       = atof(argv[3]);
        T_off      = atof(argv[4]);
        A          = atof(argv[5]);
        omega      = atof(argv[6]);
        total_time = atof(argv[7]);
        dt         = atof(argv[8]);
        gamma      = atof(argv[9]);
        kBT        = atof(argv[10]);
    }

    // Potencial asimétrico compartido
    AsymmetricPotential pot(1.0, 1.0, 0.8);

    // Crear motores Flashing y Rocking
    auto flashing = std::make_shared<FlashingMotor>(1.0, gamma, kBT, T_on, T_off, pot);
    auto rocking  = std::make_shared<RockingMotor>(1.0, gamma, kBT, A, omega, pot);

    // Condiciones iniciales
    flashing->Inicie(0.0, 0.0);
    rocking->Inicie(0.3, 0.0);

    // Sistema acoplado
    CoupledMotors system(flashing, rocking, kc, d0);

    // Archivos de salida
    std::string out_traj   = "results/motores_x1x2.dat";
    std::string out_energy = "results/motores_energia.dat";

    extern void run_coupled_simulation(CoupledMotors&, double, double,
                                       const std::string&, const std::string&, bool);

    run_coupled_simulation(system, dt, total_time, out_traj, out_energy, true);

    std::cout << "Datos guardados en:\n";
    std::cout << "  - " << out_traj << "\n";
    std::cout << "  - " << out_energy << "\n";

    // ============================
    //   GENERACIÓN DE GRÁFICAS
    // ============================
    std::cout << "\nGenerando gráficas automáticamente...\n";

    int status = std::system("python3 scripts/plot_motores.py");

    if (status == 0) {
        std::cout << "Gráficas generadas correctamente en carpeta results/.\n";
    } else {
        std::cout << "ADVERTENCIA: no se pudo ejecutar plot_motores.py\n";
        std::cout << "Verifique que python3 está instalado.\n";
    }

    return 0;
}
