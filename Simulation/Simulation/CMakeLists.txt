cmake_minimum_required(VERSION 4.1)

set(SOURCES
    BehaviorObject.cpp
    Behavior.cpp
    "Component.cpp"
    CommandQueue.cpp
    components/Planet.cpp
    behaviors/rungeKutta4.cpp
    Simulation.cpp
    RenderingCache.cpp
)

add_library(simulation STATIC ${SOURCES})

target_link_libraries(simulation PRIVATE math gpuInterface helpers)