# register custom components
# list(APPEND required_components more_components)


#TODO: Display controller support:  https://github.com/lvgl/lv_port_esp32

idf_component_register(
    SRCS "math_helper.c"
    INCLUDE_DIRS "." "./include"
    REQUIRES "${required_components}"
)