project(IconGenerator VERSION 0.0.1)

set(TargetName ${PROJECT_NAME})

juce_add_console_app(${TargetName}
        PRODUCT_NAME "MIDI Router Icon Generator")

target_sources(${TargetName} PRIVATE Main.cpp)
set_default_app_config(${TargetName})

target_link_libraries(${TargetName} PRIVATE
        ea_midirouter_ui_utils)