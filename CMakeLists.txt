# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# register custom components
list(APPEND EXTRA_COMPONENTS components/math_helper)
list(APPEND EXTRA_COMPONENTS components/arduino-esp32)
list(APPEND EXTRA_COMPONENTS components/idf-tft_espi)
list(APPEND EXTRA_COMPONENTS components/number-display)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(mqtt-controlled-led)
