# math
list(APPEND required_components math_helper)

# led_control
list(APPEND required_components nvs_flash)

# wifi
list(APPEND required_components esp_wifi)

# mqtt
list(APPEND required_components json mqtt)


idf_component_register(
    SRCS "blink.c"
    INCLUDE_DIRS "." "./include"
    PRIV_REQUIRES "${required_components}"
)