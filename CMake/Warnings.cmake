#needed to solve a Mac warning in JUCE, hopefully they fix it soon
#and we can just use juce_recommended_warning_flags

add_library(ea_warning_flags INTERFACE)

if ((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
    target_compile_options(ea_warning_flags INTERFACE "/W4")
elseif ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
    target_compile_options(ea_warning_flags INTERFACE
            -Wall
            -Wshadow-all
            -Wshorten-64-to-32
            -Wstrict-aliasing
            -Wuninitialized
            -Wunused-parameter
            -Wconversion
            -Wsign-compare
            -Wint-conversion
            -Wconditional-uninitialized
            -Wconstant-conversion
            -Wsign-conversion
            -Wbool-conversion
            -Wextra-semi
            -Wunreachable-code
            -Wcast-align
            -Wshift-sign-overflow
            -Wmissing-prototypes
            -Wnullable-to-nonnull-conversion
            -Wno-ignored-qualifiers
            -Wpedantic
            -Wdeprecated
            -Wfloat-equal
            -Wmissing-field-initializers
            $<$<OR:$<COMPILE_LANGUAGE:CXX>,$<COMPILE_LANGUAGE:OBJCXX>>:
            -Wzero-as-null-pointer-constant
            -Wunused-private-field
            -Woverloaded-virtual
            -Wreorder
            -Winconsistent-missing-destructor-override>
            $<$<OR:$<COMPILE_LANGUAGE:OBJC>,$<COMPILE_LANGUAGE:OBJCXX>>:
            -Wunguarded-availability
            -Wunguarded-availability-new>)
endif ()

add_library(ea_default_config INTERFACE)
target_link_libraries(ea_default_config INTERFACE
        ea_warning_flags
        juce_recommended_config_flags
        juce_recommended_lto_flags)