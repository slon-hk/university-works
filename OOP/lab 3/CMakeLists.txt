cmake_minimum_required(VERSION 3.20)
project(OOP_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Type build" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SDL лежит в vendor/; если его нет рядом, берём общий из lab 1
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/CMakeLists.txt)
    add_subdirectory(vendor)
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lab\ 1/vendor ${CMAKE_BINARY_DIR}/vendor)
endif()

# SDL передаёт путь к exports через "SHELL:", и пробел в пути ("lab 1") ломает линковку
foreach(sdl_target SDL3-shared SDL3_ttf-shared)
    if(TARGET ${sdl_target})
        get_target_property(sdl_link_options ${sdl_target} LINK_OPTIONS)
        if(sdl_link_options)
            list(TRANSFORM sdl_link_options REPLACE "^SHELL:(-Wl,-exported_symbols_list,.*)$" "\\1")
            set_target_properties(${sdl_target} PROPERTIES LINK_OPTIONS "${sdl_link_options}")
        endif()
    endif()
endforeach()
add_subdirectory(src)
