#include <iostream>     // Para std::cout y std::endl
#include <Eigen/Dense>  // Cabecera “todo en uno” de Eigen (matrices, vectores, etc.)

int main() {
    // Matriz A de tamaño 2x2 con elementos de tipo double.
    // Sintaxis: Matrix<tipo, filas, columnas>
    Eigen::Matrix<double, 2, 2> A;

    // Operador de inserción '<<' de Eigen para rellenar por filas:
    // A = [ 1 2
    //       3 4 ]
    A << 1, 2,
         3, 4;

    // Vector columna b de tamaño 2x1 (también de tipo double).
    Eigen::Matrix<double, 2, 1> b;

    // b = [5
    //      6]
    b << 5,
         6;

    // --- Opción directa (tu versión) ---
    // x = A^{-1} * b
    // Nota: calcular la inversa explícita es menos estable y más costoso.
    Eigen::Matrix<double, 2, 1> x = A.inverse() * b;

    // Imprimir la solución. Eigen sobrecarga << para mostrar matrices/vectores.
    std::cout << "Solución del sistema Ax = b:\n" << x << std::endl;

    return 0;
}
