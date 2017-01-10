
project(collatz-server)

#YARP needs Cmake 2.8.9
cmake_minimum_required(VERSION 2.8.9)

#find YARP
find_package(YARP REQUIRED)

#add YARP include directories
include_directories(include ${YARP_INCLUDE_DIRS})

#set up my program
add_executable(server main.cpp Server.cpp FIFO.cpp)

#link with YARP libraries
target_link_libraries(server ${YARP_LIBRARIES})