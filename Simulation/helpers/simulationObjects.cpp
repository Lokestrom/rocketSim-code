#include "simulationObjects.hpp"

namespace timeObjects {
    void updateTime() noexcept {
        currentTime += dt;
        dtInstancesSinceLastLogging++;
    }
}