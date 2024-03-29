#pragma once
#include "led_diode.h"
#include "initializable_struct.h"


#define RGB_LED_DIODE_T__DISPLAY_NAME__BUFFER_SIZE 5

/**
 * @brief aggregate root for an RGB LED consisting of 3 channel leds
 */
typedef struct
{
    INITIALIZABLE_STRUCT;
    uint8_t id; // Uniquely identifies the RGB led in the system.
    // Display name that is used to adress the LED by name (in logs and messages).
    char display_name[RGB_LED_DIODE_T__DISPLAY_NAME__BUFFER_SIZE];
    led_diode_t red;   // Red LED of the RGB LED
    led_diode_t green; // Green LED of the RGB LED
    led_diode_t blue;  // Blue LED of the RGB LED
} rgb_led_diode_t;

/**
 * @brief a bag filled with RGB LED diodes. A bag does
 *        not guarantee any order but provides a size
 *        for simpler iteration and handling.
 */
typedef struct
{
    INITIALIZABLE_STRUCT;
    rgb_led_diode_t *leds; // An array of RGB LEDs
    uint8_t count;          // The size of the RGB LED array
} rgb_led_diode_bag_t;
