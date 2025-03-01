cmake_minimum_required(VERSION 3.15)
project(plotter LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# generate compile json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Specify the compiler explicitly (use GCC or Clang)
set(CMAKE_C_COMPILER ${CC})

# Other CMake configuration...
# Include Raylib headers
include_directories(${RAYLIB_INCLUDE})
include_directories("/nix/store/lw21wr626v5sdcaxxkv2k4zf1121hfc9-glibc-2.40-36-dev/include")

# Source files
file(GLOB SRC_FILES "*.c")

message(STATUS "Raylib Include Dir: ${RAYLIB_INCLUDE}")

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# add_library(mylibrary STATIC lorenz.c)

# Link Raylib and other necessary libraries
target_link_libraries(${PROJECT_NAME} raylib GL m pthread dl rt X11)
