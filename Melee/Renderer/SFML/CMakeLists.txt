cmake_minimum_required (VERSION 3.8)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML REQUIRED COMPONENTS system window graphics network audio)

add_library (SFMLRenderer
    SFMLRenderer.cpp
    SFMLRenderer.hpp
	SFMLRenderable.hpp
	SFMLUtils.hpp
    Entities/SFMLAsteroidEntityRenderer.cpp
    Entities/SFMLAsteroidEntityRenderer.hpp
    Entities/SFMLExhaustEntityRenderer.cpp
    Entities/SFMLExhaustEntityRenderer.hpp
	Entities/SFMLExplosionEntityRenderer.cpp
    Entities/SFMLExplosionEntityRenderer.hpp
    Entities/SFMLPlanetEntityRenderer.cpp
	Entities/SFMLPlanetEntityRenderer.hpp
    Entities/SFMLPlayerEntityRenderer.cpp
	Entities/SFMLPlayerEntityRenderer.hpp
    Entities/SFMLProjectileEntityRenderer.cpp
	Entities/SFMLProjectileEntityRenderer.hpp
)

target_include_directories (SFMLRenderer PRIVATE ../../)
target_compile_features (SFMLRenderer PUBLIC cxx_std_17)
target_link_libraries (SFMLRenderer PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio Engine)
