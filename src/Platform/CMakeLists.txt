# CMakeLists.txt for Platformlib
cmake_minimum_required( VERSION 2.6 )

# X11 Library
find_package( X11 REQUIRED )

# XCB Library
find_package( XCB REQUIRED )
find_package( X11_XCB REQUIRED )

# libudev - linux only
find_package( UDev REQUIRED )
find_package( EvDev REQUIRED )

set( PROCYON_PLATFORM_SRC_FILES )
if ( PROCYON_LINUX )
	add_subdirectory( Unix/ )
endif()

add_library(Procyon_Platform STATIC
	Window.cpp
	Joystick.cpp
	${PROCYON_PLATFORM_SRC_FILES}
)

target_include_directories(Procyon_Platform PUBLIC
	${X11_INCLUDE_DIR}
	${X11_XCB_INCLUDE_DIR}
	${XCB_INCLUDE_DIRS}
	${UDEV_INCLUDE_DIR}
	${EVDEV_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Unix
)

# Adds preprocessor definitions.
target_compile_definitions(Procyon_Platform PUBLIC
	${LIBXCB_DEFINITIONS}
	${X11_XCB_DEFINITIONS}
)

target_link_libraries(Procyon_Platform
	Procyon_Common
	Procyon_Graphics
	${X11_LIBRARIES}
	${X11_XCB_LIBRARIES}
	${XCB_LIBRARIES} 
	${UDEV_LIBRARIES}
	${EVDEV_LIBRARIES}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Platform PARENT_SCOPE )
