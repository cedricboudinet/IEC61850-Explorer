CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
project(iec61850Explorer)
option(BUILD_GUI "Build GUI (Requires Qt5)" ON)

add_executable(iec61850Explorer iec61850Explorer.C)
target_link_libraries(iec61850Explorer iec61850 pthread)

add_executable(iec61850_varCmd iec61850_varCmd.C)
target_link_libraries(iec61850_varCmd iec61850 pthread)

add_executable(iec61850_testsrv iec61850_testsrv.c)
target_link_libraries(iec61850_testsrv iec61850 pthread)

IF(BUILD_GUI)
	find_package(Qt5Widgets REQUIRED)
	include_directories(${Qt5Widgets_INCLUDES})
	add_executable(iec61850ExplorerGUI iec61850ExplorerGUI.C)
	qt5_use_modules(iec61850ExplorerGUI Widgets)
	set_target_properties(iec61850ExplorerGUI PROPERTIES AUTOMOC TRUE)
ENDIF(BUILD_GUI)

