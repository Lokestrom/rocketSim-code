cmake_minimum_required(VERSION 4.1)

set(SOURCES
	Model.cpp
)

add_library(gpuInterface STATIC ${SOURCES})

target_link_libraries(gpuInterface PRIVATE glm glad)

target_include_directories(gpuInterface PRIVATE ${CMAKE_SOURCE_DIR}/Filesystem/external/)
