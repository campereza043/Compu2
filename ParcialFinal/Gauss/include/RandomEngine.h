#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

#include <random>
#include <cstdint>

/**
 * @brief Wrapper simple para motor de números aleatorios (mt19937).
 *
 * Provee un generador de reales uniformes en (0,1).
 */
class RandomEngine {
public:
    explicit RandomEngine(uint32_t seed = 12345u);
    double uniform(); // devuelve en (0,1)
private:
    std::mt19937 engine_;
    std::uniform_real_distribution<double> dist_;
};

#endif // RANDOMENGINE_H
