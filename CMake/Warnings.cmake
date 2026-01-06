add_library(ea_default_config INTERFACE)
target_link_libraries(ea_default_config INTERFACE
        juce_recommended_warning_flags
        juce_recommended_config_flags
        juce_recommended_lto_flags)