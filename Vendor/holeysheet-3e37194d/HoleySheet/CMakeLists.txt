cmake_minimum_required (VERSION 3.18)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML REQUIRED COMPONENTS graphics)

add_executable (HoleySheet
    SpritePacker.cpp
)

target_include_directories (HoleySheet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features (HoleySheet PUBLIC cxx_std_17)
target_link_libraries (HoleySheet PUBLIC sfml-graphics)

if (MSVC)
    target_compile_options (HoleySheet PUBLIC /W3 /WX)
else ()
    target_compile_options (HoleySheet PUBLIC -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wshadow -Wdouble-promotion)
endif ()
