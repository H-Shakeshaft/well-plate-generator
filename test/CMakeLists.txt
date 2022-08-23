cmake_minimum_required(VERSION 3.22)

set(This WellGenTests)

set(Sources
        ExampleTest.cpp)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
        ${CONAN_LIBS}
        gtest_main
        well-gen_lib
        )

add_test(
        NAME ${This}
        COMMAND ${This}
)
