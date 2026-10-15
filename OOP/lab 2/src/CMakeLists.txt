add_executable(${PROJECT_NAME}
        robot/includes/Relation.h
        robot/includes/Robot.h
        robot/Robot.cpp
        robot/includes/Robot_factory.h
        robot/Robot_factory.cpp
        board/includes/Cell_status_generator.h
        board/Cell_status_generator.cpp
        board/includes/Direction.h
        board/includes/Game_board.h
        board/Game_board.cpp
        game/includes/Demo_level.h
        game/Demo_level.cpp
        game/includes/Enemy_controller.h
        game/Enemy_controller.cpp
        game/includes/Event_log.h
        game/Event_log.cpp
        game/includes/Game_session.h
        game/Game_session.cpp
        game/includes/Map_editor.h
        game/Map_editor.cpp
        abilities/includes/Ability_catalog.h
        abilities/Ability_catalog.cpp
        abilities/includes/Around_attack.h
        abilities/Around_attack.cpp
        abilities/includes/Heal.h
        abilities/Heal.cpp
        abilities/includes/Long_attack.h
        abilities/Long_attack.cpp
        abilities/includes/Robot_abilities.h
        abilities/includes/Teleport.h
        abilities/Teleport.cpp
        statuses/includes/Base_status.h
        statuses/Base_status.cpp
        statuses/includes/Burn.h
        statuses/Burn.cpp
        statuses/includes/Item_pickup.h
        statuses/Item_pickup.cpp
        statuses/includes/Overload.h
        statuses/Overload.cpp
        statuses/includes/Robot_status.h
        statuses/includes/Shield.h
        statuses/Shield.cpp
        statuses/includes/Slow.h
        statuses/Slow.cpp
        statuses/includes/Status_list.h
        statuses/Status_list.cpp
        statuses/includes/Strength.h
        statuses/Strength.cpp
        items/includes/Attack_potion.h
        items/Attack_potion.cpp
        items/includes/Energy_potion.h
        items/Energy_potion.cpp
        items/includes/Health_potion.h
        items/Health_potion.cpp
        items/includes/Inventory.h
        items/Inventory.cpp
        items/includes/Item.h
        gui/includes/Assets.h
        gui/Assets.cpp
        gui/includes/Board_view.h
        gui/Board_view.cpp
        gui/includes/Canvas.h
        gui/Canvas.cpp
        gui/includes/Dialog_view.h
        gui/Dialog_view.cpp
        gui/includes/Display_mode.h
        gui/includes/Display_mode_selector.h
        gui/Display_mode_selector.cpp
        gui/includes/Gui.h
        gui/Gui.cpp
        gui/includes/Hud_view.h
        gui/Hud_view.cpp
        gui/includes/Layout.h
        gui/Layout.cpp
        gui/includes/Palette.h
        gui/Palette.cpp
        gui/includes/Player_controller.h
        gui/Player_controller.cpp
        gui/includes/Sprite_catalog.h
        gui/Sprite_catalog.cpp
        game/Game_template.cpp
        game/includes/Game_template.h
        game/includes/Level_result.h
)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/robot
    ${CMAKE_CURRENT_SOURCE_DIR}/board
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/abilities
    ${CMAKE_CURRENT_SOURCE_DIR}/statuses
    ${CMAKE_CURRENT_SOURCE_DIR}/items
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
)

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    FONT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/fonts/Samson.ttf"
    ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM
)
