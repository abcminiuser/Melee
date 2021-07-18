cmake_minimum_required (VERSION 3.8)

list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../Vendor)

set(SDL2_GFX_ROOT "${CMAKE_CURRENT_LIST_DIR}/src")

set(SDL2_GFX_INCLUDE_DIRS "${SDL2_GFX_ROOT}")

add_library(SDL2_gfx
	${SDL2_GFX_ROOT}/SDL2_framerate.c
	${SDL2_GFX_ROOT}/SDL2_gfxPrimitives.c
	${SDL2_GFX_ROOT}/SDL2_imageFilter.c
	${SDL2_GFX_ROOT}/SDL2_rotozoom.c
)

set(SDL2_GFX_LIBRARIES SDL2_gfx)

target_link_libraries(SDL2_gfx PRIVATE SDL2)
target_include_directories(SDL2_gfx PUBLIC ${SDL2_GFX_INCLUDE_DIRS})

add_library(SDL2::GFX ALIAS SDL2_gfx)
