project(MIDIRouter VERSION 0.0.1)

set (TargetName ${PROJECT_NAME})

read_plist_cache(AppInfo.plist appInfoPlist)
juce_add_gui_app(${TargetName}
        PRODUCT_NAME "MIDI Router"
        PLIST_TO_MERGE ${appInfoPlist})

target_sources(${TargetName} PRIVATE Main.cpp)

target_compile_definitions(${TargetName} PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${TargetName},JUCE_PROJECT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${TargetName},JUCE_VERSION>")

target_link_libraries(${TargetName} PRIVATE
        ea_default_config
        juce_gui_extra
        juce_audio_utils
        ea_data_structures
        JSON)