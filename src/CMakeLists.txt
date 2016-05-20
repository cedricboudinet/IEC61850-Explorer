add_library (iec61850Exp STATIC iec61850Exp_fun.cpp MmsValueWrapper.cpp)
target_link_libraries(iec61850Exp pthread iec61850)

add_executable(iec61850-shell iec61850-shell.cpp)
target_link_libraries(iec61850-shell iec61850 iec61850Exp)

add_executable(iec61850-scan iec61850-scan.cpp)
target_link_libraries(iec61850-scan iec61850 iec61850Exp)

add_executable(iec61850_varCmd iec61850_varCmd.cpp)
target_link_libraries(iec61850_varCmd iec61850 iec61850Exp)

add_executable(iec61850_testsrv iec61850_testsrv.c)
target_link_libraries(iec61850_testsrv iec61850 pthread)
set(RES_FILES "")
if( MINGW )
	set(RES_FILES "${CMAKE_CURRENT_BINARY_DIR}/version.rc")
	message(STATUS "Generating RC file : ${RES_FILES}")
	configure_file(
			${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
			${RES_FILES}
			@ONLY)
	set(CMAKE_RC_COMPILER_INIT windres)
	ENABLE_LANGUAGE(RC)
	SET(CMAKE_RC_COMPILE_OBJECT
	"<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

IF(BUILD_GUI)
	find_package(Qt5Widgets REQUIRED)
	include_directories(${Qt5Widgets_INCLUDES})
	qt5_add_resources(guiresources icons.qrc)
	add_executable(iec61850-Explorer WIN32 iec61850-Explorer.cpp VariablesListWindow.cpp ExplorerWindow.cpp VariablesView.cpp ${RES_FILES} ${guiresources} ../include/VariablesView.h ../include/VariablesListWindow.h ../include/ExplorerWindow.h)
	qt5_use_modules(iec61850-Explorer Widgets)
	set_target_properties(iec61850-Explorer PROPERTIES AUTOMOC TRUE)
	target_link_libraries(iec61850-Explorer iec61850 iec61850Exp)
ENDIF(BUILD_GUI)
if(WIN32)
	set(LIBDEP_4_WIN wpcap ws2_32 iphlpapi)
	target_link_libraries(iec61850-shell ${LIBDEP_4_WIN})
	target_link_libraries(iec61850-Explorer ${LIBDEP_4_WIN})
	target_link_libraries(iec61850_testsrv ${LIBDEP_4_WIN})
	target_link_libraries(iec61850_varCmd ${LIBDEP_4_WIN})
endif()

IF(BUILD_GUI)
	INSTALL(TARGETS iec61850-Explorer RUNTIME DESTINATION bin COMPONENT Applications)
ENDIF(BUILD_GUI)
INSTALL(TARGETS iec61850-shell RUNTIME DESTINATION bin COMPONENT Applications)
INSTALL(TARGETS iec61850_varCmd RUNTIME DESTINATION bin COMPONENT Applications)
IF(WIN32)
	#Packaging Qt5 libs:
	GET_TARGET_PROPERTY(QT5Core_LIB_LOCATION Qt5::Core LOCATION_${BUILD_TYPE})
	GET_TARGET_PROPERTY(QT5Gui_LIB_LOCATION Qt5::Gui LOCATION_${BUILD_TYPE})
	GET_TARGET_PROPERTY(QT5Widgets_LIB_LOCATION Qt5::Widgets LOCATION_${BUILD_TYPE})
	INSTALL(FILES ${QT5Core_LIB_LOCATION} ${QT5Gui_LIB_LOCATION} ${QT5Widgets_LIB_LOCATION} DESTINATION bin COMPONENT Applications)
	get_filename_component(QT_DLL_DIR ${QT5Core_LIB_LOCATION} PATH)
	install(FILES
			${QT_DLL_DIR}/../plugins/platforms/qwindows.dll
			DESTINATION bin/platforms COMPONENT Applications
		   )

	#Packaging third party librairies
	find_library(LIBIEC61850_LIBRARY NAMES libiec61850 PATHS ${CMAKE_SOURCE_DIR}/third_party/libiec61850/bin)
	message(STATUS "Found LIBIEC61850 in ${LIBIEC61850_LIBRARY}")
	find_library(WPCAP_LIBRARY NAMES wpcap PATHS third_party/winpcap/lib)
	message(STATUS "Found Winpcap in ${WPCAP_LIBRARY}")
	find_library(PACKET_LIBRARY NAMES packet PATHS third_party/winpcap/lib)
	INSTALL(FILES ${PACKET_LIBRARY} ${LIBIEC61850_LIBRARY} ${WPCAP_LIBRARY} DESTINATION bin COMPONENT Applications)

	if( MINGW )
		message( STATUS "Installing system-libraries: MinGW DLLs." )
		get_filename_component( Mingw_Path ${CMAKE_CXX_COMPILER} PATH )
		set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_dw2-1.dll ${Mingw_Path}/libstdc++-6.dll ${Mingw_Path}/libwinpthread-1.dll)
	endif( MINGW )

	if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
		install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin COMPONENT Applications)
	endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
ENDIF(WIN32)
