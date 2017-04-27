project(2d_pong)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

file(GLOB SOURCES src/*.cpp src/*.hpp)
ADD_EXECUTABLE(pong ${SOURCES})
TARGET_LINK_LIBRARIES(pong ${CONAN_LIBS})

set (CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "pong")

FILE(COPY assets/ DESTINATION bin/assets)
FILE(COPY src/shaders DESTINATION bin/)
