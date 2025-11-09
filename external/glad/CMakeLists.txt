cmake_minimum_required(VERSION 4.1)

set(SOURCES
	glad.c
)

add_library(glad STATIC ${SOURCES})

# The glad headers (glad.h and the KHR/ folder) live directly in this folder.
# Export the folder itself so consumers can #include <glad/glad.h> and glad.h
# can include <KHR/khrplatform.h> successfully.
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})