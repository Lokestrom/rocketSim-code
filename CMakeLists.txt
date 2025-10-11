cmake_minimum_required(VERSION 4.1)

project(RocketSimulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_VULKAN "Build with Vulkan backend" OFF)

add_subdirectory(external)

set(FileSystem
    simulation/FileSystem/FileSystem.cpp
    simulation/FileSystem/LoadManager.cpp
    simulation/FileSystem/logging.cpp
    simulation/FileSystem/RocketInstructions.cpp
)

set(glad
    simulation/glad/glad.c
)

file(GLOB_RECURSE OpenGL CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Simulation/openGL/*.cpp"
)

set(helpers
    simulation/helpers/controles.cpp
    simulation/helpers/ID.cpp
    simulation/helpers/math.cpp
    simulation/helpers/Mesh.cpp
    simulation/helpers/physics.cpp
    simulation/helpers/simulationObjects.cpp
    simulation/helpers/TransformComponent3D.cpp
    simulation/helpers/math/Vector3.cpp
    simulation/helpers/math/Vector2.cpp
    simulation/helpers/math/matrix2.cpp
    simulation/helpers/math/Quaternion.cpp
)

set(rocket
    simulation/rocket/Rocket.cpp
    simulation/rocket/Engine.cpp
    simulation/rocket/FuelTank.cpp
    simulation/rocket/RocketStage.cpp
    simulation/rocket/SimulationObject.cpp
    simulation/rocket/FuelMap.cpp
)

file(GLOB_RECURSE vulkan CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Simulation/vulkan/*.cpp"
)

set(general
    simulation/DragSim.cpp
    simulation/main.cpp
    simulation/ModelCash.cpp
    simulation/ObjectRenderingCashing.cpp
    simulation/Planet.cpp
    simulation/run.cpp
    simulation/SimulationModel.cpp
)

add_executable(app ${FileSystem} ${helpers} ${rocket} ${general})

set_property(TARGET app PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Simulation")

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Simulation)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Simulation/glad)

#rest of these is a hacky solution to fix include path problems
#TODO fix this
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Simulation/openGL)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Simulation/openGL/windows)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Simulation/openGL/windows/core)

if(USE_VULKAN)
    target_compile_definitions(app PRIVATE VulkanRendering)
else()
    target_compile_definitions(app PRIVATE OpenGLRendering)
endif()
#TODO: make these only be needed when the specific rendering methode is used
    target_sources(app PRIVATE ${vulkan})
    target_sources(app PRIVATE ${glad} ${OpenGL})

target_link_libraries(app PRIVATE glfw glm Database)
#if(USE_VULKAN)
    target_link_libraries(app PRIVATE freetype Vulkan::Vulkan)
#endif()
