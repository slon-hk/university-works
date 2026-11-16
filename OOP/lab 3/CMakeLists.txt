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
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/CMakeLists.txt")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor")
else()
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../lab 1/vendor" "${CMAKE_BINARY_DIR}/vendor")
endif()

# SDL генерирует -Wl,-exported_symbols_list для файла .exports. При пути с пробелом
# (например, "lab 1") этот вариант ломается. Преобразуем в безопасный Xlinker-формат.
foreach(sdl_target SDL3-shared SDL3_ttf-shared)
    if(TARGET ${sdl_target})
        get_target_property(sdl_link_options ${sdl_target} LINK_OPTIONS)
        if(sdl_link_options)
            set(cleaned_link_options "")
            foreach(opt IN LISTS sdl_link_options)
                string(REGEX REPLACE "^SHELL:-Wl,-exported_symbols_list,(.*)$" "-Xlinker;-exported_symbols_list;-Xlinker;\\1" fixed_opt "${opt}")
                list(APPEND cleaned_link_options "${fixed_opt}")
            endforeach()
            set_target_properties(${sdl_target} PROPERTIES LINK_OPTIONS "${cleaned_link_options}")
        endif()
    endif()
endforeach()
add_subdirectory(src)
