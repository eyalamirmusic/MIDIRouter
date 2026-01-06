project(MIDIRouter VERSION 0.0.1)

set(TargetName ${PROJECT_NAME})
set(IconPNG ${CMAKE_CURRENT_LIST_DIR}/MIDIRouterIcon.png)

read_plist_cache(AppInfo.plist appInfoPlist)
juce_add_gui_app(${TargetName}
        PRODUCT_NAME "MIDI Router"
        PLIST_TO_MERGE ${appInfoPlist}
        ICON_BIG ${IconPNG}
        ICON_SMALL ${IconPNG})

target_sources(${TargetName} PRIVATE Main.cpp)

set_default_app_config(${TargetName})

target_link_libraries(${TargetName} PRIVATE
        juce_gui_extra
        ea_midirouter_ui_utils
        juce_audio_utils
        ea_data_structures
        JSON)