cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

set(CMAKE_C_STANDARD 11)

# Set source, object, and binary directories
set(SRC_DIR src)
set(OBJ_DIR obj)
set(BIN_DIR bin)

# Find source files
file(GLOB SOURCES ${SRC_DIR}/*.c)

# Create an executable
add_executable(chip8 ${SOURCES})

# Include directories
target_include_directories(chip8 PRIVATE include)

# Set compiler flags
target_compile_options(chip8 PRIVATE -Wall -g -DDEBUG)

# Set linker flags
target_link_libraries(chip8 PRIVATE m ncurses GL GLU glut pthread)

# Create output directories
file(MAKE_DIRECTORY ${OBJ_DIR})
file(MAKE_DIRECTORY ${BIN_DIR})
