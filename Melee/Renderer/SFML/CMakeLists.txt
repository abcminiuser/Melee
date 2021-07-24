cmake_minimum_required (VERSION 3.8)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML REQUIRED COMPONENTS system window graphics network audio)

add_library (SFMLRenderer
    SFMLRenderer.cpp
    SFMLRenderer.hpp
    SFMLExhaustEntityRenderer.cpp
    SFMLExhaustEntityRenderer.hpp
    SFMLPlanetEntityRenderer.cpp
	SFMLPlanetEntityRenderer.hpp
    SFMLPlayerEntityRenderer.cpp
	SFMLPlayerEntityRenderer.hpp
	SFMLRenderable.hpp
	SFMLUtils.hpp
)

target_include_directories (SFMLRenderer PRIVATE ../../)
target_compile_features (SFMLRenderer PUBLIC cxx_std_17)
target_link_libraries (SFMLRenderer PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio Engine)
