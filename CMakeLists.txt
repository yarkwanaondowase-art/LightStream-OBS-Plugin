cmake_minimum_required(VERSION 3.28)
project(lightstream VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OBS plugin builds normally receive libobs from the OBS build/development tree.
# The GitHub workflow supplies OBS_SOURCE_DIR and builds against that tree.
if(NOT OBS_SOURCE_DIR)
  message(FATAL_ERROR "OBS_SOURCE_DIR must point to an OBS Studio source/build environment")
endif()

add_library(lightstream MODULE src/lightstream.cpp)

target_include_directories(lightstream PRIVATE
  "${OBS_SOURCE_DIR}/libobs"
  "${OBS_SOURCE_DIR}/libobs/util"
  "${OBS_SOURCE_DIR}/libobs/graphics"
)

if(TARGET libobs)
  target_link_libraries(lightstream PRIVATE libobs)
else()
  find_library(LIBOBS_LIBRARY NAMES obs libobs PATHS "${OBS_BUILD_DIR}/libobs" "${OBS_BUILD_DIR}/libobs/RelWithDebInfo" REQUIRED)
  target_link_libraries(lightstream PRIVATE "${LIBOBS_LIBRARY}")
endif()

if(WIN32)
  set_target_properties(lightstream PROPERTIES PREFIX "" OUTPUT_NAME "lightstream")
endif()
