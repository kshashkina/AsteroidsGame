cmake_minimum_required(VERSION 3.26)
project(oop_game)

set(CMAKE_CXX_STANDARD 17)

# Include the adirectory where the SFML include files are located
include_directories("C:/KSE IT/oop_game/SFML-2.6.1/include")

# Tell CMake where to find the SFML library files
link_directories("C:/KSE IT/oop_game/SFML-2.6.1/lib")

# Define the executable target
add_executable(oop_game main.cpp)

# Link the SFML libraries to the executable target
target_link_libraries(oop_game sfml-graphics sfml-window sfml-system)
