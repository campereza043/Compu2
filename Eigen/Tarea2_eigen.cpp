// eigen_menu.cpp
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <complex>

using namespace std;
using namespace Eigen;

// --- Helpers de entrada con validación básica ---
double read_double(const string &prompt) {
    string line;
    double value;
    while (true) {
        cout << prompt;
        if (!std::getline(cin, line)) { // EOF
            cout << "\nEntrada inesperada. Saliendo.\n";
            exit(0);
        }
        // quitar espacios
        std::stringstream ss(line);
        if (ss >> value) {
            // asegurar que no haya caracteres extra
            double extra;
            if (!(ss >> extra)) return value;
        }
        cout << "Entrada inválida. Intente de nuevo (ingrese un número).\n";
    }
}

Vector3d read_vector3(const string &name) {
    Vector3d v;
    cout << "Ingrese los 3 componentes de " << name << ":\n";
    for (int i = 0; i < 3; ++i) {
        v[i] = read_double("  componente " + to_string(i+1) + ": ");
    }
    return v;
}

Matrix2d read_matrix2(const string &name) {
    Matrix2d M;
    cout << "Ingrese los 4 elementos de la matriz 2x2 " << name << " (fila por fila):\n";
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            M(i,j) = read_double("  elemento (" + to_string(i+1) + "," + to_string(j+1) + "): ");
    return M;
}

Matrix3d read_matrix3(const string &name) {
    Matrix3d M;
    cout << "Ingrese los 9 elementos de la matriz 3x3 " << name << " (fila por fila):\n";
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            M(i,j) = read_double("  elemento (" + to_string(i+1) + "," + to_string(j+1) + "): ");
    return M;
}

// Comprobación rápida de igualdad simétrica con tolerancia
bool is_symmetric(const Matrix2d &M, double tol = 1e-9) {
    return (M - M.transpose()).cwiseAbs().maxCoeff() < tol;
}

bool is_symmetric3(const Matrix3d &M, double tol = 1e-9) {
    return (M - M.transpose()).cwiseAbs().maxCoeff() < tol;
}

// --- Operaciones solicitadas ---
void op_vector_sum() {
    cout << "\n-- Suma de vectores 3D --\n";
    Vector3d a = read_vector3("vector A");
    Vector3d b = read_vector3("vector B");
    Vector3d s = a + b;
    cout << "Vector A = [" << a.transpose() << "]\n";
    cout << "Vector B = [" << b.transpose() << "]\n";
    cout << "A + B    = [" << s.transpose() << "]\n";
}

void op_dot_product() {
    cout << "\n-- Producto punto (dot) de vectores 3D --\n";
    Vector3d a = read_vector3("vector A");
    Vector3d b = read_vector3("vector B");
    double dot = a.dot(b);
    cout << "A · B = " << dot << "\n";
}

void op_cross_product() {
    cout << "\n-- Producto cruz (cross) de vectores 3D --\n";
    Vector3d a = read_vector3("vector A");
    Vector3d b = read_vector3("vector B");
    Vector3d c = a.cross(b);
    cout << "A x B = [" << c.transpose() << "]\n";
}

void op_matrix2_sum() {
    cout << "\n-- Suma de matrices 2x2 --\n";
    Matrix2d A = read_matrix2("A");
    Matrix2d B = read_matrix2("B");
    Matrix2d S = A + B;
    cout << "Matriz A:\n" << A << "\n\n";
    cout << "Matriz B:\n" << B << "\n\n";
    cout << "A + B =\n" << S << "\n";
}

void op_matrix2_mult() {
    cout << "\n-- Multiplicación de matrices 2x2 (A * B) --\n";
    Matrix2d A = read_matrix2("A");
    Matrix2d B = read_matrix2("B");
    Matrix2d P = A * B;
    cout << "A * B =\n" << P << "\n";
}

void op_matrix2_symmetric_check() {
    cout << "\n-- Verificar si una matriz 2x2 es simétrica --\n";
    Matrix2d M = read_matrix2("M");
    if (is_symmetric(M)) cout << "Es simétrica\n";
    else cout << "No es simétrica\n";
}

void op_diagonalize_symmetric_3() {
    cout << "\n-- Diagonalizar matriz 3x3 simétrica --\n";
    Matrix3d M = read_matrix3("M (debe ser simétrica)");
    if (!is_symmetric3(M)) {
        cout << "La matriz ingresada NO es simétrica (o difiere por más que la tolerancia). Operación cancelada.\n";
        return;
    }
    SelfAdjointEigenSolver<Matrix3d> es(M);
    if (es.info() != Success) {
        cout << "Error al diagonalizar la matriz.\n";
        return;
    }
    cout << "Autovalores (vector):\n" << es.eigenvalues() << "\n\n";
    cout << "Autovectores (columnas):\n" << es.eigenvectors() << "\n\n";
    cout << "Verificación: V * diag(lambda) * V^T =\n" 
         << es.eigenvectors() * es.eigenvalues().asDiagonal() * es.eigenvectors().transpose() << "\n";
}

void op_eigenvalues_2x2() {
    cout << "\n-- Autovalores de matriz cuadrada 2x2 (pueden ser complejos) --\n";
    Matrix2d M = read_matrix2("M");
    EigenSolver<Matrix2d> es(M);
    if (es.info() != Success) {
        cout << "Error al calcular autovalores.\n";
        return;
    }
    VectorXcd vals = es.eigenvalues();
    cout << "Autovalores:\n";
    for (int i = 0; i < vals.size(); ++i) {
        complex<double> v = vals[i];
        cout << "  lambda_" << i+1 << " = " << v << "\n";
    }
}

// --- Menú interactivo ---
void print_menu() {
    cout << "\n=== MENÚ DE OPERACIONES ===\n";
    cout << "1  - Sumar dos vectores 3D\n";
    cout << "2  - Producto punto de dos vectores 3D\n";
    cout << "3  - Producto cruz de dos vectores 3D\n";
    cout << "4  - Sumar dos matrices 2x2\n";
    cout << "5  - Multiplicar dos matrices 2x2 (A * B)\n";
    cout << "6  - Verificar si una matriz 2x2 es simétrica\n";
    cout << "7  - Diagonalizar una matriz 3x3 simétrica (autovalores y autovectores)\n";
    cout << "8  - Mostrar autovalores (posible complejos) de una matriz 2x2\n";
    cout << "Seleccione una opción: ";
}

int main() {
    while (true) {
        print_menu();
        string line;
        if (!getline(cin, line)) break;
        int opt;
        stringstream ss(line);
        if (!(ss >> opt)) {
            cout << "Opción inválida. Ingrese un número entre 1 y 8.\n";
            continue;
        }

        switch (opt) {
            case 1: op_vector_sum(); break;
            case 2: op_dot_product(); break;
            case 3: op_cross_product(); break;
            case 4: op_matrix2_sum(); break;
            case 5: op_matrix2_mult(); break;
            case 6: op_matrix2_symmetric_check(); break;
            case 7: op_diagonalize_symmetric_3(); break;
            case 8: op_eigenvalues_2x2(); break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
        }
    }
    return 0;
}
