cmake_minimum_required (VERSION 3.8)

set(SDL2_ROOT "${CMAKE_CURRENT_LIST_DIR}/src")

set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/include")

if(WIN32)
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
		set(SDL2_LIBRARIES "${SDL2_ROOT}/lib/x64/SDL2.lib;${SDL2_ROOT}/lib/x64/SDL2main.lib")
		set(SDL2_DLLS  "${SDL2_ROOT}/lib/x64/SDL2.dll")
	else ()
		set(SDL2_LIBRARIES "${SDL2_ROOT}/lib/x86/SDL2.lib;${SDL2_ROOT}/lib/x86/SDL2main.lib")
		set(SDL2_DLLS  "${SDL2_ROOT}/lib/x86/SDL2.dll")
	endif ()
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

add_library(SDL2 INTERFACE)
target_link_libraries(SDL2 INTERFACE ${SDL2_LIBRARIES})
target_include_directories(SDL2 INTERFACE ${SDL2_INCLUDE_DIRS})
