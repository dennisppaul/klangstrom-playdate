cmake_minimum_required(VERSION 3.19)
project("klangstrom-playdate")

set(KLANGSTROM_PLAYDATE_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/klangstrom-playdate)

if (NOT DEFINED ENV{PLAYDATE_SDK_PATH})
    message("PLAYDATE_SDK_PATH is not set or is empty ...")
    set(ENV{PLAYDATE_SDK_PATH} "$ENV{HOME}/Developer/PlaydateSDK")
    message("... setting it to $ENV{PLAYDATE_SDK_PAT}")
endif ()

# C++ 20 and up is required in order to include the playdate headers
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# add playdate-cpp project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/playdate-cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/klangwellen)

# application
add_playdate_application(${PROJECT_NAME})

# sources
FILE(GLOB APP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(${PROJECT_NAME} PUBLIC ${APP_SRC_FILES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# playdate library
target_sources(${PROJECT_NAME} PUBLIC ${KLANGSTROM_PLAYDATE_LIBRARY}/src/main.cpp)
include_directories(${KLANGSTROM_PLAYDATE_LIBRARY}/include)

# link library
target_link_libraries(${PROJECT_NAME} pdcpp_core)
modify_target_for_playdate(klangwellen_library)
target_link_libraries(${PROJECT_NAME} klangwellen_library)
