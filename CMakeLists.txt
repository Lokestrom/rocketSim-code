cmake_minimum_required(VERSION 4.1)

project(RocketSimulation LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(OpenGLRendering)
add_compile_definitions(
  SHADER_DIR="${CMAKE_SOURCE_DIR}/simulation/ui/openGL/shaders/"
  DEFAULT_CONFIG="${CMAKE_SOURCE_DIR}/simulation/FileSystem/default/config/"
)

set(SOURCES
	Simulation/main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Simulation)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/database/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

add_subdirectory(external)
add_subdirectory(simulation/gpuInterface)
add_subdirectory(simulation/helpers)
add_subdirectory(simulation/helpers/math)
add_subdirectory(simulation/Simulation)
add_subdirectory(simulation/ui/ImGui)
add_subdirectory(simulation/ui/openGL)

add_executable(app ${SOURCES})
target_link_libraries(app PRIVATE OpenGLApp)

set_property(TARGET app PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Simulation")