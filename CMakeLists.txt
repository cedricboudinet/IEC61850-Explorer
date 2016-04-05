CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
project(iec61850Explorer)
add_executable(iec61850Explorer iec61850Explorer.C)
target_link_libraries(iec61850Explorer iec61850 pthread)
