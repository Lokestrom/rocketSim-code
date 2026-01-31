cmake_minimum_required(VERSION 4.1)

set(SOURCES
	matrix2.cpp
	Quaternion.cpp
	Vector2.cpp
	Vector3.cpp
)

add_library(math STATIC ${SOURCES})

target_link_libraries(math PRIVATE Database)