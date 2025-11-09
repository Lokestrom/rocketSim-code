cmake_minimum_required(VERSION 4.1)

set(SOURCES
	controles.cpp
	ID.cpp
	"math.cpp"
	physics.cpp
	simulationObjects.cpp
	TransformComponent3D.cpp
)

add_library(helpers STATIC ${SOURCES})

target_link_libraries(helpers PRIVATE math)