cmake_minimum_required(VERSION 3.5)

project(pixel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_TESTS "Build tests" ON)



find_package(SFML COMPONENTS graphics CONFIG REQUIRED)
find_package(GTest REQUIRED)


add_library(lib STATIC ${CMAKE_CURRENT_SOURCE_DIR}/include/matrix.h)

add_executable(pixel main.cpp)

target_include_directories(lib PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "${SFML_ROOT}/include"
)


target_link_libraries(pixel PUBLIC sfml-graphics)

if(BUILD_TESTS)
  add_executable(tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
  )
  target_link_libraries(tests GTest::gtest_main ${OPENGL_LIBRARIES} sfml-graphics)
  enable_testing()
  add_test(NAME unit_tests COMMAND tests)
endif()
