cmake_minimum_required(VERSION 3.17)
project(helloTriangle)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(libs)

add_executable(helloTriangle main.cpp)
target_link_libraries(helloTriangle glfw3.dll libEGL.dll libGLESv2.dll d3dcompiler_47.dll)