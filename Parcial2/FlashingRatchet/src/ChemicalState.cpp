#include "../include/ChemicalState.h"

ChemicalState::ChemicalState(double t_on, double t_off) 
    : period_on(t_on), period_off(t_off), time_in_state(0), state(1) {}

void ChemicalState::update(double dt) {
    time_in_state += dt;
    if (state == 1 && time_in_state >= period_on) {
        state = 0;
        time_in_state = 0;
    } else if (state == 0 && time_in_state >= period_off) {
        state = 1;
        time_in_state = 0;
    }
}

int ChemicalState::getState() const { return state; }