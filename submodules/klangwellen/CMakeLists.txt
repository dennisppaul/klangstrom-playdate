project("Klangwellen")

message(STATUS "Adding Klangwellen")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(klangwellen_library INTERFACE
        ../../klangstrom-playdate/include/Crank.h)
target_include_directories(klangwellen_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/klangwellen/include)
target_compile_definitions(klangwellen_library INTERFACE KLANG_SAMPLING_RATE=48000)
target_compile_definitions(klangwellen_library INTERFACE KLANG_SAMPLES_PER_AUDIO_BLOCK=256)
