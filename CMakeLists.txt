cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME well-gen)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

set(Sources
        src/template/template.cpp src/template/template.hpp
        src/template/parser.cpp src/template/parser.hpp
        src/template/factory.cpp src/template/factory.h
        src/fuzzer/fuzzer.cpp src/fuzzer/fuzzer.hpp)

add_library(${PROJECT_NAME}_lib STATIC ${Sources})

add_executable(${PROJECT_NAME} src/main.cpp ${Sources})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_subdirectory(test)
