cmake_minimum_required(VERSION 3.19)
project("klangstrom-playdate")

# C++ 20 and up is required in order to include the playdate headers
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# klangstrom
add_compile_definitions(KLANG_SAMPLES_PER_AUDIO_BLOCK=256)
include_directories("/Users/dennisppaul/Documents/dev/klangstrom/git/klangstrom/klangwellen/include")
FILE(GLOB KLANG_WELLEN_SRC_FILES /Users/dennisppaul/Documents/dev/klangstrom/git/klangstrom/klangwellen/src/*.cpp)


# As this is part of the repo, we can't demonstrate this aptly, but for a full
# project, you will want to include this project in order to use the build
# system, which might looks something like this:
#
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/playdate-cpp)

# Now we can declare our application
add_playdate_application(klangstrom-playdate)

# Add its sources
target_sources(klangstrom-playdate PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${KLANG_WELLEN_SRC_FILES}
)

# Link against the pdcpp_core library, and you're good to go!
target_link_libraries(klangstrom-playdate pdcpp_core)
