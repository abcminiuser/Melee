cmake_minimum_required (VERSION 3.8)

add_library (Engine
    Engine.cpp
    Engine.hpp
    EngineTypes.hpp
    Entity.cpp
    Entity.hpp

    Entities/AsteroidEntity.cpp
    Entities/AsteroidEntity.hpp
    Entities/ExhaustEntity.cpp
    Entities/ExhaustEntity.hpp
    Entities/ExplosionEntity.cpp
    Entities/ExplosionEntity.hpp
    Entities/PlanetEntity.cpp
    Entities/PlanetEntity.hpp
    Entities/PlayerEntity.cpp
    Entities/PlayerEntity.hpp
    Entities/ProjectileEntity.cpp
    Entities/ProjectileEntity.hpp
)

target_include_directories (Engine PRIVATE ../)
target_compile_features (Engine PUBLIC cxx_std_17)
target_compile_definitions (Engine PUBLIC _USE_MATH_DEFINES)
