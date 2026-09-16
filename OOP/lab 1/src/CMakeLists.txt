add_executable(${PROJECT_NAME}
        Robot.cpp
        Game_board.cpp
        GameSession.cpp
        MapEditor.cpp
        GUI.cpp
        include/GUI.h
        include/Game_board.h
        include/GameSession.h
        include/MapEditor.h
        include/Direction.h
        include/Robot.h
        Robot_factory.cpp
        include/Robot_factory.h
)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/fonts/Samson.ttf"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM
)
