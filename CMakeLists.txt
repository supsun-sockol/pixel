cmake_minimum_required(VERSION 3.5)

project(pixel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS graphics CONFIG REQUIRED)


add_executable(pixel main.cpp)



target_include_directories(${PROJECT_NAME} PUBLIC
    "${SFML_ROOT}/include"
)



target_link_libraries(pixel PUBLIC sfml-graphics)
