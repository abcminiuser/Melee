cmake_minimum_required (VERSION 3.8)

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../Vendor)
    set(SFML_STATIC_LIBRARIES TRUE)
endif ()

find_package (SFML REQUIRED COMPONENTS window graphics network audio)
set (SFML_LIBRARIES sfml-window sfml-graphics sfml-network sfml-audio)

add_library (SFMLAudio
    SFMLAudio.cpp
    SFMLAudio.hpp
)

target_include_directories (SFMLAudio PRIVATE ../../)
target_compile_features (SFMLAudio PUBLIC cxx_std_17)
target_link_libraries (SFMLAudio PUBLIC ${SFML_LIBRARIES} Engine)
