cmake_minimum_required(VERSION 3.19)
project("klangstrom-playdate")

# set klangstrom playdate variable and add source files
set(KLANGSTROM_PLAYDATE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/klangstrom-playdate)
include(${KLANGSTROM_PLAYDATE_LIBRARY}/klangstrom-playdate.cmake)

# add application source files
FILE(GLOB APP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(${PROJECT_NAME} PUBLIC ${APP_SRC_FILES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
