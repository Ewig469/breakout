cmake_minimum_required(VERSION 3.16)
project(breakout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(RAYLIB_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

file(GLOB BREAKOUT_SOURCES src/*.cpp)
add_executable(breakout ${BREAKOUT_SOURCES})
target_link_libraries(breakout raylib)

# Copy picture assets to build directory after build
add_custom_command(TARGET breakout POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/picture"
    "$<TARGET_FILE_DIR:breakout>/picture"
)