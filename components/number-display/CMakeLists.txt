# creates a list of dependencies used bt components (#include esp_system.h, ...)
# These are resolved using the COMPONENTS_DIR path that points, by default to IDF_PATH/components
#    powershell -c ls $env:IDF_PATH/components
list(APPEND required_components idf-tft_espi)

idf_component_register(
    SRCS "NumberDisplay.cpp" 
    INCLUDE_DIRS "." "./include"
    REQUIRES ${required_components}
)
