cmake_minimum_required (VERSION 3.8)

list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../Vendor)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory("Engine")
add_subdirectory("Renderer")

add_executable (Melee
    Main.cpp
)

target_include_directories(Melee PRIVATE ${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Melee PUBLIC ${SDL2_LIBRARIES} PRIVATE Engine Renderer)
target_compile_features(Melee PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(Melee PRIVATE /W3 /WX)
else()
    target_compile_options(Melee PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Melee POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Melee>)
    endforeach()
endif()
