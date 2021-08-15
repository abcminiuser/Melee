cmake_minimum_required (VERSION 3.8)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML REQUIRED COMPONENTS graphics)

add_executable (SpritePacker
    SpritePacker.cpp
)

target_include_directories (SpritePacker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features (SpritePacker PUBLIC cxx_std_17)
target_link_libraries (SpritePacker PUBLIC sfml-graphics)

if (MSVC)
    target_compile_options (SpritePacker PUBLIC /W3 /WX)
else ()
    target_compile_options (SpritePacker PUBLIC -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wshadow -Wdouble-promotion)
endif ()
