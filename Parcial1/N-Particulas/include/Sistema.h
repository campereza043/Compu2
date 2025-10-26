#ifndef SISTEMA_H
#define SISTEMA_H

#include "Bola.h"
#include "Caja.h"
#include <vector>
#include <fstream>

/**
 * @class Sistema
 * @brief Gestiona la simulación completa.
 *
 * Responsabilidad: Configurar la caja, inicializar las bolas,
 * ejecutar el bucle de simulación y guardar los datos.
 */
class Sistema {
private:
  Caja caja;
  std::vector<Bola> bolas;

public:
  /**
   * @brief Define las dimensiones de la caja.
   * @param W Ancho de la caja.
   * @param H Alto de la caja.
   */
  void DefinaCaja(double W, double H);

  /**
   * @brief Reserva memoria para N bolas.
   * @param N Número de bolas.
   */
  void Reserve(int N);

  /**
   * @brief Inicializa las bolas en una rejilla regular con velocidades aleatorias.
   * @param m Masa de las bolas.
   * @param r Radio de las bolas.
   * @param vmax Magnitud máxima de la velocidad inicial.
   */
  void InicialiceRejilla(double m, double r, double vmax);

  /**
   * @brief Ejecuta un paso de la simulación.
   * @param dt Paso de tiempo.
   */
  void Paso(double dt);

  /**
   * @brief Escribe el encabezado en el archivo de salida.
   * @param f Flujo de archivo de salida.
   */
  void Encabezado(std::ofstream &f);

  /**
   * @brief Guarda el estado actual de todas las bolas en el archivo.
   * @param f Flujo de archivo de salida.
   * @param t Tiempo actual de la simulación.
   */
  void Guarde(std::ofstream &f, double t);
};

#endif // SISTEMA_H
