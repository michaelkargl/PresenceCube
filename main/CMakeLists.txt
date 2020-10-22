# math
list(APPEND required_components math_helper)

# led_control
list(APPEND required_components nvs_flash)

# wifi
list(APPEND required_components esp_wifi)

# mqtt
list(APPEND required_components json mqtt)

# eventing
list(APPEND required_components freertos)

list(APPEND srcs "blink.c")
list(APPEND srcs "wifi_setup.c")
list(APPEND srcs "mqtt_setup.c")
list(APPEND srcs "zube_ledc.c")

idf_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "." "./include"
    PRIV_REQUIRES "${required_components}"
)