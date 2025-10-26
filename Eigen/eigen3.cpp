// matrices_basico.cpp
#include <iostream>
#include <iomanip>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

// ---------- PROTOTIPOS ----------
void leerDimensiones(int& filas, int& cols, const string& nombre);
MatrixXd leerMatriz(int filas, int cols, const string& nombre);
void imprimir(const MatrixXd& M, const string& titulo);

// ---------- PROGRAMA PRINCIPAL ----------
int main() {
    cout << "=== Operaciones basicas con matrices (Eigen) ===\n\n";

    // 1) Leer dimensiones
    int fA, cA, fB, cB;
    leerDimensiones(fA, cA, "A");
    leerDimensiones(fB, cB, "B");

    // 2) Leer matrices A y B
    MatrixXd A = leerMatriz(fA, cA, "A");
    MatrixXd B = leerMatriz(fB, cB, "B");

    // 3) Suma y resta (misma dimension)
    cout << "\n--- Suma y Resta ---\n";
    if (fA == fB && cA == cB) {
        MatrixXd S = A + B;
        MatrixXd R = A - B;
        imprimir(S, "A + B");
        imprimir(R, "A - B");
    } else {
        cout << "Para sumar y restar, A y B deben tener la MISMA dimension (mismas filas y columnas).\n";
        cout << "Dimension de A: " << fA << "x" << cA << " ; "
             << "Dimension de B: " << fB << "x" << cB << "\n";
    }

    // 4) Multiplicacion (columnas de A = filas de B)
    cout << "\n--- Multiplicacion ---\n";
    if (cA == fB) {
        MatrixXd P = A * B;
        imprimir(P, "A * B");
    } else {
        cout << "Para multiplicar A*B, el numero de COLUMNAS de A debe ser igual al numero de FILAS de B.\n";
        cout << "Columnas de A: " << cA << " ; Filas de B: " << fB << "\n";
    }

    cout << "\nFin.\n";
    return 0;
}

// ---------- DEFINICIONES DE FUNCIONES ----------
void leerDimensiones(int& filas, int& cols, const string& nombre) {
    cout << "Ingrese filas y columnas de la matriz " << nombre << " (por ejemplo: 2 3): ";
    cin >> filas >> cols;
}

MatrixXd leerMatriz(int filas, int cols, const string& nombre) {
    MatrixXd M(filas, cols);
    cout << "Introduzca los " << filas*cols
         << " valores de la matriz " << nombre << " por filas:\n";
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> M(i, j);
        }
    }
    return M;
}

void imprimir(const MatrixXd& M, const string& titulo) {
    cout << "\n" << titulo << ":\n";
    cout << fixed << setprecision(6) << M << "\n";
}
