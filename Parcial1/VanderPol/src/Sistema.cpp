#include "Sistema.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cmath>

Sistema::Sistema(double mu1, double w01, double mu2, double w02, double k_) :
    o1(mu1, w01), o2(mu2, w02), k(k_) {}


void Sistema::system_derivatives(const double state[4], double deriv[4]) const {
    // state = [x1, v1, x2, v2]
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


void Sistema::rk4_step(double /*t*/, double dt, double state[4]) const {
    double k1[4], k2[4], k3[4], k4[4], tmp[4];

    system_derivatives(state, k1);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + 0.5 * dt * k1[i];
    system_derivatives(tmp, k2);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + 0.5 * dt * k2[i];
    system_derivatives(tmp, k3);

    for (int i = 0; i < 4; ++i) tmp[i] = state[i] + dt * k3[i];
    system_derivatives(tmp, k4);

    for (int i = 0; i < 4; ++i) state[i] += dt * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]) / 6.0;
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

    double state[4];
    state[0] = x10;
    state[1] = v10;
    state[2] = x20;
    state[3] = v20;

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

    double state[4];
    state[0] = x10; state[1] = v10; state[2] = x20; state[3] = v20;
    double t = t0;

    // We'll detect zero-crossing linear interpolation between steps
    double prev_x1 = state[0];
    double prev_v1 = state[1];
    double prev_state[4]; for (int i=0;i<4;++i) prev_state[i]=state[i];

    while (t <= tf + 1e-12) {
        rk4_step(t, dt, state);
        t += dt;

        double cur_x1 = state[0];
        double cur_v1 = state[1];

        // crossing from negative to positive (prev_x1 < 0 && cur_x1 >=0) or viceversa
        if (prev_x1 <= 0.0 && cur_x1 > 0.0 && cur_v1 > 0.0) {
            // linear interp to approximate crossing time
            double alpha = (0.0 - prev_x1) / (cur_x1 - prev_x1);
            double tcross = t - dt + alpha * dt;
            double x1_cross = 0.0;
            double v1_cross = prev_v1 + alpha * (cur_v1 - prev_v1);
            double x2_cross = prev_state[2] + alpha * (state[2] - prev_state[2]);
            double v2_cross = prev_state[3] + alpha * (state[3] - prev_state[3]);
            ofs << tcross << " " << x1_cross << " " << v1_cross << " " << x2_cross << " " << v2_cross << "\n";
        }

        for (int i=0;i<4;++i) prev_state[i]=state[i];
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
    double state[4];
    state[0]=x10; state[1]=v10; state[2]=x20; state[3]=v20;

    // initial tangent vector (small random or along axis) - use eps0 in x1 component
    double q[4] = {eps0, eps0*0.5, eps0*0.3, eps0*0.2};

    auto norm = [](const double v[4]) {
        return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
    };

    double qnorm = norm(q);
    // normalize q
    for (int i=0;i<4;++i) q[i] /= qnorm;

    double t = t0;
    double accum_log = 0.0;
    int renorm_steps = std::max(1, int(std::round(renorm_time / dt)));
    int step = 0;
    int nren = 0;

    // We evolve state and tangent vector using RK4: tangent evolves as dq/dt = J(x(t)) * q
    while (t <= tf + 1e-12) {
        // advance both one dt using RK4 with current Jacobian evaluated at intermediate states
        // We'll implement a coupled RK4: for the tangent we need Jacobian at the intermediate states.
        // For simplicity: do full RK4 for state; store intermediate states to compute Jacobian for tangent.
        // Step 1: compute k1 for state
        double k1s[4];
        system_derivatives(state, k1s);

        // compute state mid1
        double state_m1[4];
        for (int i=0;i<4;++i) state_m1[i] = state[i] + 0.5*dt*k1s[i];

        double k2s[4];
        system_derivatives(state_m1, k2s);

        double state_m2[4];
        for (int i=0;i<4;++i) state_m2[i] = state[i] + 0.5*dt*k2s[i];

        double k3s[4];
        system_derivatives(state_m2, k3s);

        double state_m3[4];
        for (int i=0;i<4;++i) state_m3[i] = state[i] + dt*k3s[i];

        double k4s[4];
        system_derivatives(state_m3, k4s);

        // update state
        double state_new[4];
        for (int i=0;i<4;++i) state_new[i] = state[i] + dt*(k1s[i] + 2.0*k2s[i] + 2.0*k3s[i] + k4s[i]) / 6.0;

        // Now tangent RK4 using Jacobian evaluated at same stages
        // helper to fill Jacobian at a given state
        auto fill_J = [&](const double st[4], double J[4][4]) {
            double x1 = st[0], v1 = st[1], x2 = st[2], v2 = st[3];
            // row order matches derivatives [dx1,dv1,dx2,dv2]
            // dx1 derivatives
            J[0][0] = 0.0; J[0][1] = 1.0; J[0][2] = 0.0; J[0][3] = 0.0;
            // dv1 derivatives
            J[1][0] = -2.0*o1.mu * x1 * v1 - o1.w0*o1.w0 - k; // ∂dv1/∂x1
            J[1][1] = o1.mu * (1.0 - x1*x1); // ∂dv1/∂v1
            J[1][2] = k; // ∂dv1/∂x2
            J[1][3] = 0.0;
            // dx2
            J[2][0] = 0.0; J[2][1] = 0.0; J[2][2] = 0.0; J[2][3] = 1.0;
            // dv2
            J[3][0] = k;
            J[3][1] = 0.0;
            J[3][2] = -2.0*o2.mu * x2 * v2 - o2.w0*o2.w0 - k;
            J[3][3] = o2.mu * (1.0 - x2*x2);
        };

        // compute k1 for q: k1q = J(state) * q
        double J1[4][4], J2[4][4], J3[4][4], J4[4][4];
        fill_J(state, J1);
        fill_J(state_m1, J2);
        fill_J(state_m2, J3);
        fill_J(state_m3, J4);

        auto matvec = [](double M[4][4], const double v[4], double out[4]) {
            for (int i=0;i<4;++i) {
                out[i] = 0.0;
                for (int j=0;j<4;++j) out[i] += M[i][j] * v[j];
            }
        };

        double k1q[4], k2q[4], k3q[4], k4q[4];
        matvec(J1, q, k1q);

        double q_m1[4];
        for (int i=0;i<4;++i) q_m1[i] = q[i] + 0.5*dt*k1q[i];
        matvec(J2, q_m1, k2q);

        double q_m2[4];
        for (int i=0;i<4;++i) q_m2[i] = q[i] + 0.5*dt*k2q[i];
        matvec(J3, q_m2, k3q);

        double q_m3[4];
        for (int i=0;i<4;++i) q_m3[i] = q[i] + dt*k3q[i];
        matvec(J4, q_m3, k4q);

        double q_new[4];
        for (int i=0;i<4;++i) q_new[i] = q[i] + dt*(k1q[i] + 2.0*k2q[i] + 2.0*k3q[i] + k4q[i]) / 6.0;

        // commit updates
        for (int i=0;i<4;++i) state[i] = state_new[i];
        for (int i=0;i<4;++i) q[i] = q_new[i];

        ++step;
        // renormalize every renorm_steps
        if (step % renorm_steps == 0) {
            double qn = norm(q);
            if (qn == 0.0) qn = 1e-16;
            // accumulate log growth
            accum_log += std::log(qn);
            ++nren;
            // renormalize q to unit vector
            for (int i=0;i<4;++i) q[i] /= qn;
            double lambda_running = accum_log / (nren * renorm_time);
            double tnow = t; // current time
            ofs_prog << tnow << " " << lambda_running << "\n";
        }

        // advance time
        // note: we advanced state by dt effectively at end of loop already
        t += dt;
    }

    ofs_prog.close();

    // final lambda estimate
    double lambda_final = 0.0;
    // avoid division by zero
    double total_time = tf - t0;
    if (total_time > 0.0) {
        lambda_final = accum_log / (total_time);
    }
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
        // return final x1 and x2
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

    // compute simple errors (abs difference vs dt3)
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
