#include "Sistema.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cmath>
#include <vector>

// Constructor
Sistema::Sistema(double mu1, double w01, double mu2, double w02, double k_) :
    o1(mu1, w01), o2(mu2, w02), k(k_) {}

void Sistema::system_derivatives(const double state[4], double deriv[4]) const {
    double x1 = state[0], v1 = state[1], x2 = state[2], v2 = state[3];

    // fuerzas de acoplamiento lineal: k*(xj - xi)
    double F1 = k * (x2 - x1);
    double F2 = k * (x1 - x2);

    double dx1dt, dv1dt, dx2dt, dv2dt;
    o1.derivatives(x1, v1, F1, dx1dt, dv1dt);
    o2.derivatives(x2, v2, F2, dx2dt, dv2dt);

    deriv[0] = dx1dt;
    deriv[1] = dv1dt;
    deriv[2] = dx2dt;
    deriv[3] = dv2dt;
}

void Sistema::rk4_step(double t, double dt, double state[4]) const {
    double k1[4], k2[4], k3[4], k4[4], tmp[4];

    system_derivatives(state, k1);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + 0.5 * dt * k1[i];
    system_derivatives(tmp, k2);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + 0.5 * dt * k2[i];
    system_derivatives(tmp, k3);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + dt * k3[i];
    system_derivatives(tmp, k4);

    for (int i = 0; i < 4; ++i) 
        state[i] += dt * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]) / 6.0;
}

void Sistema::simulate(double t0, double tf, double dt, const std::string &outfile,
                       double x10, double v10, double x20, double v20) const {

    std::filesystem::path outpath(outfile);
    std::filesystem::create_directories(outpath.parent_path());

    std::ofstream ofs(outfile);
    if (!ofs) {
        std::cerr << "No se pudo abrir archivo de salida: " << outfile << std::endl;
        return;
    }

    ofs << "# t x1 v1 x2 v2" << std::endl;
    ofs << std::fixed << std::setprecision(8);

    double state[4] = {x10, v10, x20, v20};
    double t = t0;

    while (t <= tf + 1e-12) {
        ofs << t << " " << state[0] << " " << state[1] << " " << state[2] << " " << state[3] << "\n";
        rk4_step(t, dt, state);
        t += dt;
    }
    ofs.close();
}

void Sistema::generate_poincare(double t0, double tf, double dt, const std::string &outfile_poincare,
                                double x10, double v10, double x20, double v20) const {

    std::filesystem::path outpath(outfile_poincare);
    std::filesystem::create_directories(outpath.parent_path());
    std::ofstream ofs(outfile_poincare);
    if (!ofs) {
        std::cerr << "No se pudo abrir archivo de Poincaré: " << outfile_poincare << std::endl;
        return;
    }
    ofs << "# t x1 v1 x2 v2 (only points where x1 crosses 0 with v1>0)\n";
    ofs << std::fixed << std::setprecision(8);

    double state[4] = {x10, v10, x20, v20};
    double t = t0;

    // Detectar cruces por cero
    double prev_x1 = state[0];
    double prev_v1 = state[1];
    double prev_state[4]; 
    for (int i=0; i<4; ++i) prev_state[i] = state[i];

    while (t <= tf + 1e-12) {
        rk4_step(t, dt, state);
        t += dt;

        double cur_x1 = state[0];
        double cur_v1 = state[1];

        // crossing from negative to positive (prev_x1 < 0 && cur_x1 >=0) with positive velocity
        if (prev_x1 <= 0.0 && cur_x1 > 0.0 && cur_v1 > 0.0) {
            // linear interpolation to approximate crossing time
            double alpha = (0.0 - prev_x1) / (cur_x1 - prev_x1);
            double tcross = t - dt + alpha * dt;
            double v1_cross = prev_v1 + alpha * (cur_v1 - prev_v1);
            double x2_cross = prev_state[2] + alpha * (state[2] - prev_state[2]);
            double v2_cross = prev_state[3] + alpha * (state[3] - prev_state[3]);
            ofs << tcross << " " << 0.0 << " " << v1_cross << " " << x2_cross << " " << v2_cross << "\n";
        }

        for (int i=0; i<4; ++i) prev_state[i] = state[i];
        prev_x1 = cur_x1;
        prev_v1 = cur_v1;
    }

    ofs.close();
}

void Sistema::compute_lyapunov(double t0, double tf, double dt, double renorm_time,
                               const std::string &outfile_progress, const std::string &outfile_final,
                               double x10, double v10, double x20, double v20, double eps0) const {

    std::filesystem::path pathprog(outfile_progress);
    std::filesystem::create_directories(pathprog.parent_path());
    std::ofstream ofs_prog(outfile_progress);
    if (!ofs_prog) {
        std::cerr << "No se pudo abrir archivo de Lyapunov (progress): " << outfile_progress << std::endl;
        return;
    }
    ofs_prog << "# time lambda_running\n";
    ofs_prog << std::fixed << std::setprecision(8);

    // initial state
    double state[4] = {x10, v10, x20, v20};

    // initial tangent vector
    double q[4] = {eps0, eps0*0.5, eps0*0.3, eps0*0.2};

    auto norm = [](const double v[4]) {
        return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
    };

    double qnorm = norm(q);
    // normalize q
    for (int i=0; i<4; ++i) q[i] /= qnorm;

    double t = t0;
    double accum_log = 0.0;
    int renorm_steps = std::max(1, int(std::round(renorm_time / dt)));
    int step = 0;
    int nren = 0;

    while (t <= tf + 1e-12) {
        // RK4 para el estado
        double k1s[4];
        system_derivatives(state, k1s);

        double state_m1[4];
        for (int i=0; i<4; ++i) state_m1[i] = state[i] + 0.5*dt*k1s[i];

        double k2s[4];
        system_derivatives(state_m1, k2s);

        double state_m2[4];
        for (int i=0; i<4; ++i) state_m2[i] = state[i] + 0.5*dt*k2s[i];

        double k3s[4];
        system_derivatives(state_m2, k3s);

        double state_m3[4];
        for (int i=0; i<4; ++i) state_m3[i] = state[i] + dt*k3s[i];

        double k4s[4];
        system_derivatives(state_m3, k4s);

        double state_new[4];
        for (int i=0; i<4; ++i) 
            state_new[i] = state[i] + dt*(k1s[i] + 2.0*k2s[i] + 2.0*k3s[i] + k4s[i]) / 6.0;

        // RK4 para el vector tangente (usando Jacobiano)
        auto fill_J = [&](const double st[4], double J[4][4]) {
            double x1 = st[0], v1 = st[1], x2 = st[2], v2 = st[3];
            // dx1 derivatives
            J[0][0] = 0.0; J[0][1] = 1.0; J[0][2] = 0.0; J[0][3] = 0.0;
            // dv1 derivatives
            J[1][0] = -2.0*o1.mu * x1 * v1 - o1.w0*o1.w0 - k;
            J[1][1] = o1.mu * (1.0 - x1*x1);
            J[1][2] = k;
            J[1][3] = 0.0;
            // dx2
            J[2][0] = 0.0; J[2][1] = 0.0; J[2][2] = 0.0; J[2][3] = 1.0;
            // dv2
            J[3][0] = k;
            J[3][1] = 0.0;
            J[3][2] = -2.0*o2.mu * x2 * v2 - o2.w0*o2.w0 - k;
            J[3][3] = o2.mu * (1.0 - x2*x2);
        };

        double J1[4][4], J2[4][4], J3[4][4], J4[4][4];
        fill_J(state, J1);
        fill_J(state_m1, J2);
        fill_J(state_m2, J3);
        fill_J(state_m3, J4);

        auto matvec = [](double M[4][4], const double v[4], double out[4]) {
            for (int i=0; i<4; ++i) {
                out[i] = 0.0;
                for (int j=0; j<4; ++j) out[i] += M[i][j] * v[j];
            }
        };

        double k1q[4], k2q[4], k3q[4], k4q[4];
        matvec(J1, q, k1q);

        double q_m1[4];
        for (int i=0; i<4; ++i) q_m1[i] = q[i] + 0.5*dt*k1q[i];
        matvec(J2, q_m1, k2q);

        double q_m2[4];
        for (int i=0; i<4; ++i) q_m2[i] = q[i] + 0.5*dt*k2q[i];
        matvec(J3, q_m2, k3q);

        double q_m3[4];
        for (int i=0; i<4; ++i) q_m3[i] = q[i] + dt*k3q[i];
        matvec(J4, q_m3, k4q);

        double q_new[4];
        for (int i=0; i<4; ++i) 
            q_new[i] = q[i] + dt*(k1q[i] + 2.0*k2q[i] + 2.0*k3q[i] + k4q[i]) / 6.0;

        // Actualizar estado y vector tangente
        for (int i=0; i<4; ++i) state[i] = state_new[i];
        for (int i=0; i<4; ++i) q[i] = q_new[i];

        ++step;
        // Renormalizar periódicamente
        if (step % renorm_steps == 0) {
            double qn = norm(q);
            if (qn == 0.0) qn = 1e-16;
            accum_log += std::log(qn);
            ++nren;
            // Renormalizar a vector unitario
            for (int i=0; i<4; ++i) q[i] /= qn;
            double lambda_running = accum_log / (nren * renorm_time);
            ofs_prog << t << " " << lambda_running << "\n";
        }

        t += dt;
    }

    ofs_prog.close();

    // Calcular Lyapunov final
    double lambda_final = accum_log / (tf - t0);
    std::ofstream ofs_final(outfile_final);
    if (!ofs_final) {
        std::cerr << "No se pudo abrir archivo de Lyapunov (final): " << outfile_final << std::endl;
        return;
    }
    ofs_final << "# lambda_final (Benettin) approx\n";
    ofs_final << std::fixed << std::setprecision(10);
    ofs_final << lambda_final << "\n";
    ofs_final.close();
}

void Sistema::validate_dt(double t0, double tf, double dt, const std::string &outfile,
                          double x10, double v10, double x20, double v20) const {

    std::filesystem::path outpath(outfile);
    std::filesystem::create_directories(outpath.parent_path());
    std::ofstream ofs(outfile);
    if (!ofs) {
        std::cerr << "No se pudo abrir archivo de validación dt: " << outfile << std::endl;
        return;
    }
    ofs << "# dt x1_final x2_final\n";
    ofs << std::fixed << std::setprecision(10);

    auto run_final = [&](double dti) {
        double state[4] = {x10, v10, x20, v20};
        double t = t0;
        while (t <= tf + 1e-12) {
            rk4_step(t, dti, state);
            t += dti;
        }
        return std::pair<double,double>(state[0], state[2]);
    };

    double dt1 = dt;
    double dt2 = dt/2.0;
    double dt3 = dt/4.0;

    auto r1 = run_final(dt1);
    auto r2 = run_final(dt2);
    auto r3 = run_final(dt3);

    ofs << dt1 << " " << r1.first << " " << r1.second << "\n";
    ofs << dt2 << " " << r2.first << " " << r2.second << "\n";
    ofs << dt3 << " " << r3.first << " " << r3.second << "\n";

    // Calcular errores vs la solución más precisa (dt/4)
    double err1_x1 = std::abs(r1.first - r3.first);
    double err2_x1 = std::abs(r2.first - r3.first);
    double err1_x2 = std::abs(r1.second - r3.second);
    double err2_x2 = std::abs(r2.second - r3.second);

    ofs << "# errors vs dt/4 (abs)\n";
    ofs << "# dt err_x1 err_x2\n";
    ofs << dt1 << " " << err1_x1 << " " << err1_x2 << "\n";
    ofs << dt2 << " " << err2_x1 << " " << err2_x2 << "\n";

    ofs.close();
}

// Nuevas funciones implementadas
void Sistema::analyze_synchronization(double t0, double tf, double dt,
                                     const std::string &outfile_sync,
                                     double x10, double v10, 
                                     double x20, double v20) const {
    
    std::filesystem::path outpath(outfile_sync);
    std::filesystem::create_directories(outpath.parent_path());
    
    std::ofstream ofs(outfile_sync);
    if (!ofs) {
        std::cerr << "Error abriendo archivo de sincronización: " << outfile_sync << std::endl;
        return;
    }

    ofs << "# t phase_diff amplitude_diff sync_index\n";
    ofs << std::fixed << std::setprecision(8);

    double state[4] = {x10, v10, x20, v20};
    double t = t0;
    
    const int window = 100;
    std::vector<double> x1_hist, x2_hist;
    
    while (t <= tf + 1e-12) {
        // Calcular diferencia de fase instantánea
        double phase1 = std::atan2(state[1], state[0]);
        double phase2 = std::atan2(state[3], state[2]);
        double phase_diff = std::abs(phase1 - phase2);
        
        // Calcular diferencia de amplitud
        double amp1 = std::sqrt(state[0]*state[0] + state[1]*state[1]);
        double amp2 = std::sqrt(state[2]*state[2] + state[3]*state[3]);
        double amp_diff = std::abs(amp1 - amp2);
        
        // Calcular índice de sincronización (correlación cruzada)
        x1_hist.push_back(state[0]);
        x2_hist.push_back(state[2]);
        
        double sync_index = 0.0;
        if (x1_hist.size() >= window) {
            // Calcular correlación cruzada en ventana deslizante
            double mean1 = 0.0, mean2 = 0.0;
            for (int i = 0; i < window; ++i) {
                mean1 += x1_hist[x1_hist.size() - window + i];
                mean2 += x2_hist[x2_hist.size() - window + i];
            }
            mean1 /= window; 
            mean2 /= window;
            
            double num = 0.0, den1 = 0.0, den2 = 0.0;
            for (int i = 0; i < window; ++i) {
                int idx = x1_hist.size() - window + i;
                double dx1 = x1_hist[idx] - mean1;
                double dx2 = x2_hist[idx] - mean2;
                num += dx1 * dx2;
                den1 += dx1 * dx1;
                den2 += dx2 * dx2;
            }
            
            if (den1 > 0 && den2 > 0) {
                sync_index = num / (std::sqrt(den1) * std::sqrt(den2));
            }
            
            // Mantener solo la ventana más reciente
            if (x1_hist.size() > window) {
                x1_hist.erase(x1_hist.begin());
                x2_hist.erase(x2_hist.begin());
            }
        }
        
        ofs << t << " " << phase_diff << " " << amp_diff << " " << sync_index << "\n";
        
        // Avanzar en el tiempo
        rk4_step(t, dt, state);
        t += dt;
    }
    
    ofs.close();
}

void Sistema::compute_power_spectrum(const std::string &datafile,
                                   const std::string &outfile_spectrum) const {
    // Esta función se implementa mejor en post-procesamiento con Python
    // Aquí solo creamos un marcador de posición
    (void)datafile; // Evitar warning de parámetro no usado
    
    std::ofstream ofs(outfile_spectrum);
    if (!ofs) {
        std::cerr << "Error creando archivo de espectro: " << outfile_spectrum << std::endl;
        return;
    }
    
    ofs << "# El análisis espectral se realiza en post-procesamiento con Python\n";
    ofs << "# Ver scripts/python/advanced_analysis.py\n";
    ofs.close();
}

void Sistema::bifurcation_analysis(double mu_min, double mu_max, int steps,
                                  double tf, double dt, 
                                  const std::string &outfile) const {
    
    std::filesystem::path outpath(outfile);
    std::filesystem::create_directories(outpath.parent_path());
    
    std::ofstream ofs(outfile);
    if (!ofs) {
        std::cerr << "Error en análisis de bifurcación: " << outfile << std::endl;
        return;
    }
    
    ofs << "# mu x1_max v1_max x2_max v2_max\n";
    ofs << std::fixed << std::setprecision(8);
    
    double dmu = (mu_max - mu_min) / (steps - 1);
    
    for (int i = 0; i < steps; ++i) {
        double mu_current = mu_min + i * dmu;
        
        // Crear sistema temporal con mu variable
        Sistema S_temp(mu_current, o1.w0, mu_current, o2.w0, k);
        
        // Simular descartando transitorio
        double state[4] = {1.0, 0.0, 0.5, 0.0};
        double t = 0.0;
        
        // Fase transitoria (descartar)
        while (t < tf / 2.0) {
            S_temp.rk4_step(t, dt, state);
            t += dt;
        }
        
        // Fase de muestreo
        double x1_max = 0.0, v1_max = 0.0, x2_max = 0.0, v2_max = 0.0;
        while (t < tf) {
            S_temp.rk4_step(t, dt, state);
            t += dt;
            
            x1_max = std::max(x1_max, std::abs(state[0]));
            v1_max = std::max(v1_max, std::abs(state[1]));
            x2_max = std::max(x2_max, std::abs(state[2]));
            v2_max = std::max(v2_max, std::abs(state[3]));
        }
        
        ofs << mu_current << " " << x1_max << " " << v1_max << " " 
            << x2_max << " " << v2_max << "\n";
    }
    
    ofs.close();
}

void Sistema::compute_jacobian(const double state[4], double J[4][4]) const {
    double x1 = state[0], v1 = state[1], x2 = state[2], v2 = state[3];
    
    J[0][0] = 0.0; J[0][1] = 1.0; J[0][2] = 0.0; J[0][3] = 0.0;
    
    J[1][0] = -2.0*o1.mu * x1 * v1 - o1.w0*o1.w0 - k;
    J[1][1] = o1.mu * (1.0 - x1*x1);
    J[1][2] = k;
    J[1][3] = 0.0;
    
    J[2][0] = 0.0; J[2][1] = 0.0; J[2][2] = 0.0; J[2][3] = 1.0;
    
    J[3][0] = k;
    J[3][1] = 0.0;
    J[3][2] = -2.0*o2.mu * x2 * v2 - o2.w0*o2.w0 - k;
    J[3][3] = o2.mu * (1.0 - x2*x2);
}