cmake_minimum_required (VERSION 3.8)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML REQUIRED COMPONENTS graphics)

add_library (SFMLRenderer
    SFMLAssetLoader.cpp
    SFMLAssetLoader.hpp
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
    Entities/SFMLShipEntityRenderer.cpp
    Entities/SFMLShipEntityRenderer.hpp
    Entities/SFMLWeaponEntityRenderer.cpp
    Entities/SFMLWeaponEntityRenderer.hpp

    UI/SFMLShipHudTileRenderer.cpp
    UI/SFMLShipHudTileRenderer.hpp
)

target_include_directories (SFMLRenderer PRIVATE ../../)
target_compile_features (SFMLRenderer PUBLIC cxx_std_17)
target_link_libraries (SFMLRenderer PUBLIC sfml-graphics Engine)
