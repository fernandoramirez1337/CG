cmake_minimum_required(VERSION 3.10)
project(MyGLFWProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the GLAD headers
include_directories(include glad/include)

# Add the GLAD source file to the project
set(GLAD_SOURCES glad/src/glad.c)

# Find the required packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Glob your source files
file(GLOB SOURCES "src/*.cpp")

# Add the executable and include GLAD source
add_executable(MyGLFWProject main.cpp ${SOURCES} ${GLAD_SOURCES})

# Link GLFW and OpenGL
target_link_libraries(MyGLFWProject PRIVATE glfw ${OPENGL_LIBRARIES})
target_include_directories(MyGLFWProject PRIVATE ${OPENGL_INCLUDE_DIR})