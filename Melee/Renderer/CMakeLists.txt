cmake_minimum_required (VERSION 3.8)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../Vendor)
else ()
    list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../Vendor/sdl2-cmake-modules/)
endif ()

find_package (SDL2 REQUIRED)
find_package (SDL2_gfx REQUIRED)

add_library (Renderer
    SDL/SDLEntityRenderer.cpp
    SDL/SDLEntityRenderer.hpp
    SDL/SDLRenderer.cpp
    SDL/SDLRenderer.hpp
)

target_include_directories (Renderer PRIVATE ../)
target_compile_features (Renderer PUBLIC cxx_std_17)
target_link_libraries (Renderer PUBLIC SDL2::Main SDL2::GFX Engine)

if (MSVC)
    foreach (DLL ${SDL2_DLLS})
        add_custom_command (TARGET Renderer POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Renderer>/..)
    endforeach ()
endif ()
