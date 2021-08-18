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
    Entities/ShipEntity.cpp
    Entities/ShipEntity.hpp
    Entities/WeaponEntity.cpp
    Entities/WeaponEntity.hpp
    Entities/Races/HumanShipEntity.cpp
    Entities/Races/HumanShipEntity.hpp

    Generators/AsteroidGenerator.cpp
    Generators/AsteroidGenerator.hpp
    Generators/EntityGenerator.cpp
    Generators/EntityGenerator.hpp
    Generators/PlanetGenerator.cpp
    Generators/PlanetGenerator.hpp

    Util/EngineUtils.hpp
    Util/VectorMath.hpp
    Util/MatrixMath.hpp
    Util/Periodic.hpp
)

target_include_directories (Engine PRIVATE ../)
target_compile_features (Engine PUBLIC cxx_std_17)
target_compile_definitions (Engine PUBLIC _USE_MATH_DEFINES)
