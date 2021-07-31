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

project ("Melee")

add_subdirectory ("Melee")
