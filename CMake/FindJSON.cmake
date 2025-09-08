include(CPM)

CPMAddPackage(NAME JSON
        URL
        "https://github.com/nlohmann/json/releases/download/v3.12.0/include.zip")

add_library(JSON INTERFACE)
target_include_directories(JSON INTERFACE ${JSON_SOURCE_DIR}/single_include)
