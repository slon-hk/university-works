# --- Настройки сборки SDL3 (задаются ДО add_subdirectory) ---
set(SDL_TEST     OFF CACHE BOOL "" FORCE)  # не собирать тесты SDL
set(SDL_SHARED   ON  CACHE BOOL "" FORCE)  # SDL как динамическая библиотека
set(SDL_STATIC   OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL)

# --- Настройки сборки SDL3_ttf (задаются ДО add_subdirectory) ---
# Собираем из исходников, чтобы SDL_ttf использовал ТОТ ЖЕ SDL3, что и проект.
set(SDLTTF_VENDORED ON  CACHE BOOL "" FORCE)  # freetype брать из вендоренных исходников
set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)  # без harfbuzz (не тянем лишние зависимости)
set(SDLTTF_PLUTOSVG OFF CACHE BOOL "" FORCE)  # без plutosvg (цветные emoji не нужны)
set(SDLTTF_SAMPLES  OFF CACHE BOOL "" FORCE)  # не собирать примеры
set(SDLTTF_INSTALL  OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL_ttf)

# --- INTERFACE-обёртка, которую подключают таргеты проекта ---
add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE SDL3::SDL3)
target_link_libraries(vendor INTERFACE SDL3_ttf::SDL3_ttf)

# SDL3::SDL3 уже даёт доступ к заголовкам через <SDL3/SDL.h>.
# Доп. путь ниже позволяет также писать короткое #include "SDL.h".
target_include_directories(vendor INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL/include/SDL3
)
