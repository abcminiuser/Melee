cmake_minimum_required (VERSION 3.8)

add_subdirectory("Engine")
add_subdirectory("Renderer")

add_executable (Melee
    Main.cpp
)

target_include_directories(Melee PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Melee PRIVATE Engine Renderer)
target_compile_features(Melee PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(Melee PUBLIC /W3 /WX)
else()
    target_compile_options(Melee PUBLIC -Wall -Wextra -Werror -Wno-unused-parameter)
endif()
