cmake_minimum_required (VERSION 3.8)

find_package(SDL2 REQUIRED)
find_package(SDL2_gfx REQUIRED)

add_library(Renderer
    SDL/SDLRenderer.cpp
    SDL/SDLRenderer.hpp
)

target_include_directories(Renderer PRIVATE ../)
target_compile_features(Renderer PUBLIC cxx_std_17)
target_compile_definitions(Renderer PUBLIC _USE_MATH_DEFINES)
target_link_libraries(Renderer PUBLIC SDL2 SDL2_gfx)
