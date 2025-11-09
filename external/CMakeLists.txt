cmake_minimum_required(VERSION 4.1)

message(STATUS "Configuring external dependencies...")

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(Database)
add_subdirectory(glad)

target_include_directories(glfw INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glfw/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_include_directories(Database INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/database/include/database)
# target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad)
