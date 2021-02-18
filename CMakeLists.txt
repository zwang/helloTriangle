cmake_minimum_required(VERSION 3.17)
project(helloTriangle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fpic -DV8_COMPRESS_POINTERS -DV8_31BIT_SMIS_ON_64BIT_ARCH")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(include)

link_directories(bin)

add_executable(helloTriangle main.cpp v8go.cc glv8.cc)
target_link_libraries(helloTriangle glfw3.dll libEGL.dll libGLESv2.dll d3dcompiler_47.dll libv8.dll libv8_libplatform.dll libwinpthread-1.dll)
