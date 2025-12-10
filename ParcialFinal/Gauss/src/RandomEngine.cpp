#include "RandomEngine.h"

RandomEngine::RandomEngine(uint32_t seed)
: engine_(seed), dist_(std::nextafter(0.0,1.0), 1.0) // evita 0 exacto
{
}

double RandomEngine::uniform() {
    return dist_(engine_);
}
