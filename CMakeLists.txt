cmake_minimum_required (VERSION 3.8)

set (CMAKE_CONFIGURATION_TYPES Debug Release)

find_program (CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
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

project ("Melee")

add_subdirectory ("Melee")

set(CPACK_PACKAGE_NAME "Melee")
set(CPACK_PACKAGE_VERSION "0.0.0")
set(CPACK_PACKAGE_VENDOR "Dean Camera")

include(CPack)
