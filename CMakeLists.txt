cmake_minimum_required(VERSION 3.7)
project(gravity)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        gravit.cpp Timer.cpp Timer.h)

find_package(sdl2 REQUIRED)

add_executable(gravity ${SOURCE_FILES})

target_include_directories(gravity PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(gravity ${SDL2_LIBRARIES} -lm)
