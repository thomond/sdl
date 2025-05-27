cmake_minimum_required(VERSION 3.14)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

# 🧩 Dependencies
find_package(Catch2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

target_link_libraries(math PUBLIC
    yaml-cpp
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_net::SDL2_net
)

# 🔧 Define your library
add_library(math src/math.cpp)
target_link_libraries(math PUBLIC yaml-cpp SDL2::SDL2)

# 🧪 Unit Tests
add_executable(tests tests/test_math.cpp)
target_link_libraries(tests PRIVATE math Catch2::Catch2WithMain yaml-cpp SDL2::SDL2)

# 🧪 Enable testing
enable_testing()
add_test(NAME math_tests COMMAND tests)
