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

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
		set_property(TARGET Melee APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif ()
else ()
    target_compile_options (Melee PUBLIC -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wshadow -Wdouble-promotion)
endif ()

add_dependencies (Melee SpritePacker)

list (APPEND IMAGE_ASSET_FOLDERS
	${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images=Images
	${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/uqm-0.8.0-content/base/planets/=Planets
	${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/uqm-0.8.0-content/base/ships/human/=Human
	${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/uqm-0.8.0-content/base/battle/=Battle
)

foreach (IMAGE_FOLDER ${IMAGE_ASSET_FOLDERS})
	string(REPLACE "=" ";" IMAGE_FOLDER_PARAMS ${IMAGE_FOLDER})
	list(GET IMAGE_FOLDER_PARAMS 0 IMAGE_FOLDER_PATH)
	list(GET IMAGE_FOLDER_PARAMS 1 IMAGE_FOLDER_SPRITESHEET_NAME)

	add_custom_command (TARGET Melee POST_BUILD
	                    COMMAND "$<TARGET_FILE:SpritePacker>"
	                    ARGS "${IMAGE_FOLDER_PATH}" "$<TARGET_FILE_DIR:Melee>/Assets/Images" "${IMAGE_FOLDER_SPRITESHEET_NAME}"
	                    COMMENT "Packing '${IMAGE_FOLDER_SPRITESHEET_NAME}' image assets...")
endforeach ()

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
