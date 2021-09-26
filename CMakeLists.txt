cmake_minimum_required(VERSION 3.10)
project(PongSFML)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("extern/SFML-2.5.1")
include_directories("extern/SFML-2.5.1/include")

set(HEADER_FILES
)

set(SOURCES
    src/main.cpp
)


add_executable(pong ${SOURCES} ${HEADER_FILES})
target_link_libraries(pong PRIVATE sfml-graphics sfml-system sfml-network sfml-audio sfml-window)
file(COPY "${CMAKE_SOURCE_DIR}/consolaz.ttf" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})