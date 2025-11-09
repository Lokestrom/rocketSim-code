cmake_minimum_required(VERSION 4.1)

set(SOURCES
	imconfig.cpp
	imgui.cpp
	imgui_draw.cpp
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_stdlib.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
)

add_library(dearImGui STATIC ${SOURCES})

# Expose headers and link dependencies so consumers don't need to add them manually.
target_include_directories(dearImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# imgui backends use GLFW and an OpenGL loader (we have glad in this project).
# Link them so dearImGui's compilation and its usage requirements are satisfied.
target_link_libraries(dearImGui
	PUBLIC glfw
)