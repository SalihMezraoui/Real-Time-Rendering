cmake_minimum_required(VERSION 3.10)
project(ProzeduralerBaum)

set(CMAKE_CXX_STANDARD 17)

# Add source files
set(SOURCES
        src/main.cpp
        src/ShaderModule.cpp
        src/ShaderPipeline.cpp
        src/DataBuffer.cpp
        src/VertexBuffer.cpp
)

set(HEADERS
        include/ShaderModule.h
        include/ShaderPipeline.h
        include/DataBuffer.h
        include/VertexBuffer.h
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find OpenGL
find_package(OpenGL REQUIRED)

# 🔹 Find GLFW
find_library(GLFW_LIB glfw3 HINTS /usr/local/Cellar/glfw/3.4/lib)
include_directories(/usr/local/Cellar/glfw/3.4/include)
link_directories(/usr/local/Cellar/glfw/3.4/lib)

# 🔹 Find GLEW
find_library(GLEW_LIB GLEW HINTS /usr/local/Cellar/glew/2.2.0_1/lib)
include_directories(/usr/local/Cellar/glew/2.2.0_1/include)
link_directories(/usr/local/Cellar/glew/2.2.0_1/lib)

# 🔹 Find GLM (Explicit Path)
include_directories(/usr/local/Cellar/glm/1.0.1/include)  # ✅ Correct path for GLM

# 🔹 Set Output Directory for Executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})  # ✅ Ensures the executable is placed in the build directory

# Add executable
add_executable(ProzeduralerBaum ${SOURCES} ${HEADERS})

# Link necessary libraries
target_link_libraries(ProzeduralerBaum
        OpenGL::GL
        ${GLFW_LIB}
        ${GLEW_LIB}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
)
