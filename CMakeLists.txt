cmake_minimum_required(VERSION 3.10)
project(Loader5095 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(CURL REQUIRED)

# Add nlohmann JSON library
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/loader.cpp
    src/display_manager.cpp
    src/graphics.cpp
    src/config_manager.cpp
    src/update_manager.cpp
    src/menu.cpp
    src/effect_renderer.cpp
)

set(HEADERS
    include/loader.h
    include/display_manager.h
    include/graphics.h
    include/config_manager.h
    include/update_manager.h
    include/menu.h
    include/effect_renderer.h
)

# Create executable
add_executable(loader5095 ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(loader5095 
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)

# Installation
install(TARGETS loader5095 DESTINATION bin)
