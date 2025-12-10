#ifndef COUPLED_MOTORS_H
#define COUPLED_MOTORS_H

#include "MotorInterface.h"
#include <memory>

class CoupledMotors {
public:
    // motor1: flashing, motor2: rocking (ambos implementan MotorInterface)
    CoupledMotors(std::shared_ptr<MotorInterface> m1,
                  std::shared_ptr<MotorInterface> m2,
                  double kc = 1.0, double d0 = 0.0)
        : motor1(m1), motor2(m2), kc(kc), d0(d0) {}

    // Accesores
    std::shared_ptr<MotorInterface> M1() { return motor1; }
    std::shared_ptr<MotorInterface> M2() { return motor2; }

    double getKc() const { return kc; }
    double getD0() const { return d0; }

private:
    std::shared_ptr<MotorInterface> motor1, motor2;
    double kc, d0;
};

#endif // COUPLED_MOTORS_H
