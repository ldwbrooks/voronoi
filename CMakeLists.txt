cmake_minimum_required(VERSION 3.10)
project(opengl_test)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)

set (EXTERNAL_DIR
    $ENV{HOMEDRIVE}/Users/$ENV{USERNAME}/Documents/coding/external/
)

set(INCLUDE_DIR ${EXTERNAL_DIR}/include/ )

set(GLAD_C_PATH ${EXTERNAL_DIR}/src/glad.c )

set(GLFW_LIB_PATH ${EXTERNAL_DIR}/lib/libglfw3.a )

add_executable(game
    src/main.cpp
    src/game.cpp
    src/line_object.cpp
    src/line_renderer.cpp
    src/resource_manager.cpp
    src/shader.cpp
    src/triangle_object.cpp
    src/triangle_renderer.cpp
    src/voronoi.cpp
    ${GLAD_C_PATH}
)
target_include_directories(game PUBLIC
                          ${INCLUDE_DIR}
                          )
target_link_libraries(game
    opengl32
    ${GLFW_LIB_PATH}
    kernel32 user32 gdi32 ws2_32
)

add_custom_command(
        TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/shaders
                ${CMAKE_CURRENT_BINARY_DIR}/shaders)