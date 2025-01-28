cmake_minimum_required(VERSION 3.16)
project(hello)

# set the output directory for built objects.

if (WIN32)

    # Define SDL2 paths manually
    set(SDL2_INCLUDE_DIR "$ENV{USERPROFILE}/SDL2mingw/include/")
    set(SDL2_LIB_DIR "$ENV{USERPROFILE}/SDL2mingw/lib")

    # Add the SDL_MAIN_HANDLED definition to avoid conflicts with SDL2's main function
    add_definitions(-DSDL_MAIN_HANDLED)

    # Add include directories
    include_directories(${SDL2_INCLUDE_DIR})

    # Link SDL2 libraries
    link_directories(${SDL2_LIB_DIR})

    # Add the main source file
    add_executable(${PROJECT_NAME} main.c)

    # Link SDL2 libraries and add -mwindows for GUI applications
    target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf)

ELSE ()

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

ENDIF ()