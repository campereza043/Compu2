// autovalores_autovectores.cpp
#include <iostream>
#include <iomanip>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace std;
using namespace Eigen;

// ---------- PROTOTIPOS ----------
MatrixXd leerMatrizCuadrada(int& n);
void mostrarAutovaloresYAutovectores(const VectorXcd& vals, const MatrixXcd& vecs);

int main() {
    cout << "=== Autovalores y Autovectores (Eigen) ===\n\n";

    // 1) Leer matriz cuadrada
    int n;
    MatrixXd A = leerMatrizCuadrada(n);

    // 2) Resolver el problema propio A v = lambda v
    EigenSolver<MatrixXd> es(A);  // Para matrices reales generales

    // 3) Obtener resultados (complejos en general)
    VectorXcd autovalores  = es.eigenvalues();
    MatrixXcd autovectores = es.eigenvectors();

    // 4) Mostrar resultados
    mostrarAutovaloresYAutovectores(autovalores, autovectores);

    cout << "\nFin.\n";
    return 0;
}

// ---------- DEFINICIONES ----------
MatrixXd leerMatrizCuadrada(int& n) {
    cout << "Ingrese n (dimension de la matriz cuadrada): ";
    cin >> n;

    MatrixXd A(n, n);
    cout << "Introduzca los " << n*n << " valores de la matriz A por filas:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A(i, j);

    return A;
}

void mostrarAutovaloresYAutovectores(const VectorXcd& vals, const MatrixXcd& vecs) {
    cout << fixed << setprecision(8);

    cout << "\n--- Autovalores (lambda) ---\n";
    for (int i = 0; i < vals.size(); ++i)
        cout << "lambda[" << i << "] = " << vals(i) << '\n';

    cout << "\n--- Autovectores (columnas) ---\n";
    for (int i = 0; i < vecs.cols(); ++i) {
        cout << "v[" << i << "] =\n" << vecs.col(i) << "\n\n";
    }

    // Nota: si A es simetrica (A = A^T), se puede usar SelfAdjointEigenSolver
    // y los resultados seran reales.
}
