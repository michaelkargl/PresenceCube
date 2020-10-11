# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# register custom components
list(APPEND EXTRA_COMPONENTS components/math_helper)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(mqtt-controlled-led)
