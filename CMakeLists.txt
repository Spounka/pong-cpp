cmake_minimum_required(VERSION 3.10)
project(PongSFML)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.5 COMPONENTS network audio graphics window system REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

set(HEADER_FILES
)

set(SOURCES
    src/main.cpp
)


add_executable(pong ${SOURCES} ${HEADER_FILES})
target_link_libraries(pong PRIVATE sfml-graphics sfml-system sfml-network sfml-audio sfml-window)