/**
 * @file ChemicalState.h
 * @brief Gestiona el estado químico del motor (ON/OFF).
 * * Simula el ciclo de hidrólisis de ATP alternando entre un estado
 * donde el potencial actúa (ON) y otro donde no (OFF) o es plano.
 */

#ifndef CHEMICALSTATE_H
#define CHEMICALSTATE_H

class ChemicalState {
private:
    double time_in_state; ///< Tiempo acumulado en el estado actual
    double period_on;     ///< Duración del estado ON
    double period_off;    ///< Duración del estado OFF
    int state;            ///< 1 = ON, 0 = OFF

public:
    /**
     * @brief Constructor
     * @param t_on Tiempo de duración del estado activo.
     * @param t_off Tiempo de duración del estado inactivo.
     */
    ChemicalState(double t_on, double t_off);

    /**
     * @brief Actualiza el reloj interno y conmuta el estado si es necesario.
     * @param dt Paso de tiempo de la simulación.
     */
    void update(double dt);

    /**
     * @brief Obtiene el estado actual.
     * @return 1 si está encendido (ON), 0 si está apagado (OFF).
     */
    int getState() const;
};

#endif