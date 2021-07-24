cmake_minimum_required (VERSION 3.8)

add_subdirectory ("Engine")
add_subdirectory ("Renderer")

add_executable (Main
    Main.cpp
)

target_include_directories (Main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (Main PRIVATE Engine SFMLRenderer)
target_compile_features (Main PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options (Main PUBLIC /W3 /WX)
else ()
    target_compile_options (Main PUBLIC -Wall -Wextra -Werror -Wno-unused-parameter)
endif ()
