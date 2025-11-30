#ifndef CHEMICALSTATE_H
#define CHEMICALSTATE_H

[cite_start]// Maneja el estado ON/OFF [cite: 70]
class ChemicalState {
private:
    double time_in_state;
    double period_on;
    double period_off;
    int state; // 1 = ON, 0 = OFF

public:
    ChemicalState(double t_on, double t_off);
    void update(double dt);
    int getState() const;
};

#endif