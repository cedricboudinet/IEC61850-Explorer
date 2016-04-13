CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
project(iec61850Explorer)
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDES})

add_executable(iec61850Explorer iec61850Explorer.C)
add_executable(iec61850getVar iec61850getVar.C)
add_executable(iec61850_testsrv iec61850_testsrv.c)
target_link_libraries(iec61850Explorer iec61850 pthread)
target_link_libraries(iec61850_testsrv iec61850 pthread)
target_link_libraries(iec61850getVar iec61850 pthread)

add_executable(iec61850ExplorerGUI iec61850ExplorerGUI.C)
qt5_use_modules(iec61850ExplorerGUI Widgets)
