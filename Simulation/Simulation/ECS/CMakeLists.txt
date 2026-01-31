cmake_minimum_required(VERSION 4.1)

set(SOURCES
    "Entity.cpp"
    Simulation.cpp
    "Integrator.cpp"
    "../Entities/PhysicsBody.cpp"
)

add_library(ECS STATIC ${SOURCES})

target_include_directories(ECS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ECS/")

target_link_libraries(ECS PRIVATE math helpers Database)