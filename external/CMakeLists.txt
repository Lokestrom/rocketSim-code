cmake_minimum_required(VERSION 4.1)

project(external)

message(STATUS "Configuring external dependencies...")

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(Database)

#if(USE_VULKAN)
    message(STATUS "Vulkan enabled — including Vulkan and FreeType")

    add_subdirectory(freetype)
    if(WIN32)
        if(DEFINED ENV{VULKAN_SDK})
            set(VULKAN_SDK_PATH "$ENV{VULKAN_SDK}")
            set(VULKAN_INCLUDE_DIR "${VULKAN_SDK_PATH}/Include")
            set(VULKAN_LIB_DIR "${VULKAN_SDK_PATH}/Lib")

            add_library(Vulkan::Vulkan STATIC IMPORTED GLOBAL)
            set_target_properties(Vulkan::Vulkan PROPERTIES
                IMPORTED_LOCATION "${VULKAN_LIB_DIR}/vulkan-1.lib"
                INTERFACE_INCLUDE_DIRECTORIES "${VULKAN_INCLUDE_DIR}"
            )
        else()
            message(WARNING "VULKAN_SDK not found! Install the Vulkan SDK, if you have done so already, make sure the environment variable VULKAN_SDK is set.")
        endif()
    else()
        find_package(Vulkan REQUIRED)
    endif()
#else()
#    message(STATUS "Vulkan disabled — skipping FreeType and Vulkan setup.")
#endif()

message(${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_include_directories(glfw INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glfw/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(glm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glm)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/database/include/database)
