cmake_minimum_required(VERSION 3.19)
project("klangstrom-playdate")
set(KLANGWELLEN_PATH "../klangstrom/klangwellen/")

# C++ 20 and up is required in order to include the playdate headers
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# add playdate-cpp project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/playdate-cpp)

# application
add_playdate_application(klangstrom-playdate)

# sources
FILE(GLOB APP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(klangstrom-playdate PUBLIC ${APP_SRC_FILES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# klangstrom
add_compile_definitions(KLANG_SAMPLES_PER_AUDIO_BLOCK=256)
include_directories(${KLANGWELLEN_PATH}/include)
FILE(GLOB KLANGWELLEN_SRC_FILES ${KLANGWELLEN_PATH}/src/*.cpp)
target_sources(klangstrom-playdate PUBLIC ${KLANGWELLEN_SRC_FILES})

# link library
target_link_libraries(klangstrom-playdate pdcpp_core)
