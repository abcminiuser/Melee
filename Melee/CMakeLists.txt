cmake_minimum_required (VERSION 3.18)

set (VENDOR_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Vendor")

add_subdirectory ("Audio")
add_subdirectory ("Engine")
add_subdirectory ("Renderer")

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

add_dependencies (Melee HoleySheet)

list (APPEND IMAGE_ASSET_FOLDERS
	${CMAKE_CURRENT_SOURCE_DIR}/Assets/Images=Core
	${VENDOR_BASE_DIR}/uqm-0.8.0-content/base/planets/=Planets
	${VENDOR_BASE_DIR}/uqm-0.8.0-content/base/ships/human/=Human
	${VENDOR_BASE_DIR}/uqm-0.8.0-content/base/ships/arilou/=Arilou
	${VENDOR_BASE_DIR}/uqm-0.8.0-content/base/ships/androsynth/=Androsynth
	${VENDOR_BASE_DIR}/uqm-0.8.0-content/base/battle/=Battle
)

list (APPEND FONT_ASSET_FOLDERS
	${VENDOR_BASE_DIR}/8bit-operator-plus=8BitOperatorPlus
)

list (APPEND MUSIC_ASSET_FOLDERS
	${VENDOR_BASE_DIR}/soundimage-music=SoundImage
)

list (APPEND SOUND_EFFECT_ASSET_FOLDERS
	${VENDOR_BASE_DIR}/soundimage-soundeffects=SoundImage
)

foreach (IMAGE_FOLDER ${IMAGE_ASSET_FOLDERS})
	string(REPLACE "=" ";" IMAGE_FOLDER_PARAMS ${IMAGE_FOLDER})
	list(GET IMAGE_FOLDER_PARAMS 0 IMAGE_FOLDER_PATH)
	list(GET IMAGE_FOLDER_PARAMS 1 IMAGE_FOLDER_NAME)

	add_custom_command (TARGET Melee POST_BUILD
	                    COMMAND "$<TARGET_FILE:HoleySheet>"
	                    ARGS "${IMAGE_FOLDER_PATH}" "$<TARGET_FILE_DIR:Melee>/Assets/Images" "${IMAGE_FOLDER_NAME}"
	                    COMMENT "Packing '${IMAGE_FOLDER_NAME}' image assets...")
endforeach ()

foreach (FONT_FOLDER ${FONT_ASSET_FOLDERS})
	string(REPLACE "=" ";" FONT_FOLDER_PARAMS ${FONT_FOLDER})
	list(GET FONT_FOLDER_PARAMS 0 FONT_FOLDER_PATH)
	list(GET FONT_FOLDER_PARAMS 1 FONT_FOLDER_NAME)

	add_custom_command (TARGET Melee POST_BUILD
	                    COMMAND ${CMAKE_COMMAND}
                    	ARGS -E copy_directory "${FONT_FOLDER_PATH}" "$<TARGET_FILE_DIR:Melee>/Assets/Fonts"
	                    COMMENT "Copying '${FONT_FOLDER_NAME}' font assets...")
endforeach ()

foreach (MUSIC_FOLDER ${MUSIC_ASSET_FOLDERS})
	string(REPLACE "=" ";" MUSIC_FOLDER_PARAMS ${MUSIC_FOLDER})
	list(GET MUSIC_FOLDER_PARAMS 0 MUSIC_FOLDER_PATH)
	list(GET MUSIC_FOLDER_PARAMS 1 MUSIC_FOLDER_NAME)

	add_custom_command (TARGET Melee POST_BUILD
	                    COMMAND ${CMAKE_COMMAND}
                    	ARGS -E copy_directory "${MUSIC_FOLDER_PATH}" "$<TARGET_FILE_DIR:Melee>/Assets/Music"
	                    COMMENT "Copying '${MUSIC_FOLDER_NAME}' music assets...")
endforeach ()

foreach (SOUND_EFFECT_FOLDER ${SOUND_EFFECT_ASSET_FOLDERS})
	string(REPLACE "=" ";" SOUND_EFFECT_FOLDER_PARAMS ${SOUND_EFFECT_FOLDER})
	list(GET SOUND_EFFECT_FOLDER_PARAMS 0 SOUND_EFFECT_FOLDER_PATH)
	list(GET SOUND_EFFECT_FOLDER_PARAMS 1 SOUND_EFFECT_FOLDER_NAME)

	add_custom_command (TARGET Melee POST_BUILD
	                    COMMAND ${CMAKE_COMMAND}
                    	ARGS -E copy_directory "${SOUND_EFFECT_FOLDER_PATH}" "$<TARGET_FILE_DIR:Melee>/Assets/SoundEffects"
	                    COMMENT "Copying '${SOUND_EFFECT_FOLDER_NAME}' sound effect assets...")
endforeach ()

install (FILES
	$<TARGET_FILE:Melee>
	$<TARGET_FILE_DIR:Melee>/Assets
	DESTINATION .
)
