cmake_minimum_required (VERSION 3.18)

set (CMAKE_CONFIGURATION_TYPES Debug Release)

find_program (CPPCHECK_BIN NAMES cppcheck)
if (CPPCHECK_BIN)
	set (CMAKE_CXX_CPPCHECK ${CPPCHECK_BIN})

    list (APPEND CMAKE_CXX_CPPCHECK
        "--enable=warning"
        "--inconclusive"
        "--force"
        "--inline-suppr"
        "--quiet"
        "--std=c++17"
    )

    message ("Using cppcheck configuration: ${CMAKE_CXX_CPPCHECK}")
endif()

if (MSVC)
    list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Vendor)
endif ()

project ("HoleySheet")

add_subdirectory ("HoleySheet")

set(CPACK_PACKAGE_NAME "Holey Sheet")
set(CPACK_PACKAGE_VERSION "0.0.0")
set(CPACK_PACKAGE_VENDOR "Dean Camera")

include(CPack)
