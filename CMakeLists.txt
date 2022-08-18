cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME well-gen)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(Sources
        )

add_executable(${PROJECT_NAME} src/main.cpp ${Sources})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
