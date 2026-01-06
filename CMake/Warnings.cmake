add_library(ea_default_config INTERFACE
        ../Modules/ea_midirouter_ui_utils/ea_midirouter_ui_utils.cpp
        ../Modules/ea_midirouter_ui_utils/ea_midirouter_ui_utils.h
        ../Modules/ea_midirouter_ui_utils/Source/TrayIcon.cpp
        ../Modules/ea_midirouter_ui_utils/Source/TrayIcon.h)
target_link_libraries(ea_default_config INTERFACE
        juce_recommended_warning_flags
        juce_recommended_config_flags
        juce_recommended_lto_flags)

function(set_default_app_config target)
    target_compile_definitions(${target} PRIVATE
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${TargetName},JUCE_PROJECT_NAME>"
            JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${TargetName},JUCE_VERSION>")

    target_link_libraries(${target} PUBLIC
            juce_recommended_warning_flags
            juce_recommended_config_flags
            juce_recommended_lto_flags)
endfunction()