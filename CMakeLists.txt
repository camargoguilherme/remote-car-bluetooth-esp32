cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(BLUEPAD32_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/components/bluepad32)
set(BTSTACK_ROOT ${BLUEPAD32_ROOT}/external/btstack)

set(EXTRA_COMPONENT_DIRS ${BLUEPAD32_ROOT}/src/components ${BTSTACK_ROOT})
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(remote_car_esp32)


