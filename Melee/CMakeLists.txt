cmake_minimum_required (VERSION 3.8)

list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../Vendor)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (Melee "Melee.cpp" "Melee.h")
target_include_directories(Melee PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Melee PUBLIC ${SDL2_LIBRARIES})

if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET Melee POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:Melee>)
    endforeach()
endif()
