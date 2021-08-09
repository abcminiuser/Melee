cmake_minimum_required (VERSION 3.8)

add_subdirectory ("Audio")
add_subdirectory ("Engine")
add_subdirectory ("Renderer")
add_subdirectory ("SpritePacker")

add_executable (Melee
    Main.cpp
)

target_include_directories (Melee PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (Melee PRIVATE Engine SFMLAudio SFMLRenderer)
target_compile_features (Melee PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options (Melee PUBLIC /W3 /WX)
else ()
    target_compile_options (Melee PUBLIC -Wall -Wextra -Werror -Wno-unused-parameter -Wshadow -Wdouble-promotion)
endif ()

add_dependencies (Melee SpritePacker)

add_custom_command (TARGET Melee PRE_BUILD
                    COMMAND "$<TARGET_FILE:SpritePacker>"
                    ARGS "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images" "$<TARGET_FILE_DIR:Melee>/Assets/Images"
                    COMMENT "Packing image assets...")

add_custom_command (TARGET Melee POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                    ARGS -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Music" "$<TARGET_FILE_DIR:Melee>/Assets/Music"
                    COMMENT "Copying music assets...")

add_custom_command (TARGET Melee POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                    ARGS -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets/SoundEffects" "$<TARGET_FILE_DIR:Melee>/Assets/SoundEffects"
                    COMMENT "Copying sound effect assets...")

add_custom_command (TARGET Melee POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                    ARGS -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Fonts" "$<TARGET_FILE_DIR:Melee>/Assets/Fonts"
                    COMMENT "Copying font assets...")

install (FILES
         $<TARGET_FILE:Melee>
         $<TARGET_FILE_DIR:Melee>/Assets
         DESTINATION .
)
