// example_sistema_lineal.cpp
// Resolver Ax = b con Eigen usando descomposición QR con pivoteo de columnas.

#include <iostream>     // cout, cin
#include <iomanip>      // setprecision, fixed
#include <Eigen/Dense>  // Cabecera principal de Eigen

using namespace std;

int main() {
    // ------------------------------------------------------------
    // 1) Leer el tamaño del sistema
    // ------------------------------------------------------------
    int n;
    cout << "Ingrese n (tamano del sistema Ax = b): ";
    cin >> n;

    // ------------------------------------------------------------
    // 2) Declarar A (n x n) y b (n) de tamano dinamico
    //    MatrixXd  = matriz de doubles de tamano dinamico
    //    VectorXd  = vector columna de doubles de tamano dinamico
    // ------------------------------------------------------------
    Eigen::MatrixXd A(n, n);
    Eigen::VectorXd b(n);

    // ------------------------------------------------------------
    // 3) Leer la matriz A por filas
    // ------------------------------------------------------------
    cout << "Introduzca la matriz A, fila por fila (" << n*n << " numeros):\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A(i, j);

    // ------------------------------------------------------------
    // 4) Leer el vector b
    // ------------------------------------------------------------
    cout << "Introduzca el vector b (" << n << " numeros):\n";
    for (int i = 0; i < n; ++i)
        cin >> b(i);

    // ------------------------------------------------------------
    // 5) Resolver Ax = b
    //    colPivHouseholderQr() es una opcion robusta para sistemas generales.
    //    Alternativas:
    //      - A.fullPivLu().solve(b)
    //      - A.colPivHouseholderQr().solve(b)
    //      - A.ldlt().solve(b)         (si A es simetrica definida positiva)
    // ------------------------------------------------------------
    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

    // ------------------------------------------------------------
    // 6) Mostrar la solucion y un chequeo del residuo ||Ax - b||
    // ------------------------------------------------------------
    cout << fixed << setprecision(8);
    cout << "\nSolucion x:\n" << x << "\n";
    double residuo = (A * x - b).norm();
    cout << "\nNorma del residuo ||Ax - b|| = " << residuo << "\n";

    return 0;
}
