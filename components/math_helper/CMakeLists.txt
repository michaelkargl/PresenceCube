# register custom components
# list(APPEND required_components more_components)

idf_component_register(
    SRCS "math_helper.c"
    INCLUDE_DIRS "." "./include"
    REQUIRES "${required_components}"
)