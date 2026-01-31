cmake_minimum_required(VERSION 4.1)

set(SOURCES
    CommandQueue.cpp
    RenderingCache.cpp
    simBenchmark.cpp
)

add_library(simulation STATIC ${SOURCES})

target_link_libraries(simulation PUBLIC math gpuInterface helpers ECS Database glad glfw)