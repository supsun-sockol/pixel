cmake_minimum_required(VERSION 3.5)

project(pixel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_TESTS "Build tests" ON)



find_package(SFML COMPONENTS graphics CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)



add_library(${PROJECT_NAME} STATIC
    include/matrix.h
    sources/matrix.cpp
    )

add_executable(main main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/shaders>"
    "${SFML_ROOT}/include"
    "${BOOST_ROOT}/include"
)


target_link_libraries(main PUBLIC
    ${PROJECT_NAME} sfml-graphics
    Boost::boost Boost::program_options
)

if(BUILD_TESTS)
  add_executable(tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
  )
  target_link_libraries(tests GTest::gtest_main ${PROJECT_NAME} ${OPENGL_LIBRARIES} sfml-graphics)
  enable_testing()
  add_test(NAME unit_tests COMMAND tests)
endif()
