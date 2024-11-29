cmake_minimum_required(VERSION 3.29)
project(Project_1_jumping_frog)

set(CMAKE_CXX_STANDARD 20)

# Set paths for ncurses
set(CMAKE_PREFIX_PATH /opt/local)
set(CMAKE_INCLUDE_PATH /opt/local/include)
set(CMAKE_LIBRARY_PATH /opt/local/lib)

# Find ncurses package
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# Define the executable and link with ncurses
add_executable(Project_1_jumping_frog main.cpp
        fun/fun.h
        fun/fun.cpp)  # Keep this line only once
target_link_libraries(Project_1_jumping_frog ${CURSES_LIBRARIES})




