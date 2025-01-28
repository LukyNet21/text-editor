cmake_minimum_required(VERSION 3.16)
project(hello)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(SDL2_INCLUDE_DIR "/libs/SDL2-2.30.11/include")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(libs/SDL2-2.30.11 EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(main WIN32 main.c)

# Link to the actual SDL3 library.
target_link_libraries(main PRIVATE SDL2::SDL2)