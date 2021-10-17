cmake_minimum_required (VERSION 3.18)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML CONFIG REQUIRED COMPONENTS audio)

add_library (SFMLAudio STATIC
    SFMLAudio.cpp
    SFMLAudio.hpp
    SFMLAudioAssetLoader.cpp
    SFMLAudioAssetLoader.hpp
)

target_include_directories (SFMLAudio PRIVATE ../../)
target_compile_features (SFMLAudio PUBLIC cxx_std_17)
target_link_libraries (SFMLAudio PUBLIC sfml-audio Engine)
