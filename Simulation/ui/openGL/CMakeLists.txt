cmake_minimum_required(VERSION 4.1)

set(SOURCES
	callbacks.cpp
	Camera.cpp
	io.cpp
	Model.cpp
	nativeOSfunctions.cpp
	OpenGLApp.cpp
	Shader.cpp

	windows/ImguiWindow.cpp
	windows/core/renderWindow.cpp
	windows/core/TimeWindow.cpp
	windows/core/EntityView.cpp
	windows/editor/testAdder.cpp
	windows/startup/Launcher.cpp

	RenderObjects/vector.cpp
)

add_library(OpenGLApp STATIC ${SOURCES})

target_link_libraries(OpenGLApp PRIVATE math simulation helpers glfw glad dearImGui)
target_link_libraries(OpenGLApp PUBLIC Database)