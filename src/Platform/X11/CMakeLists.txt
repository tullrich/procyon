# X11 Library
find_package( X11 REQUIRED )

# XCB Library
find_package( XCB REQUIRED )
find_package( X11_XCB REQUIRED )

# libudev - linux only
find_package( UDev REQUIRED )
find_package( EvDev REQUIRED )

set( PROCYON_SRCS
	${PROCYON_SRCS}
	${CMAKE_CURRENT_SOURCE_DIR}/X11GLContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/X11GLContext.h
	${CMAKE_CURRENT_SOURCE_DIR}/X11Window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/X11Window.h
	${CMAKE_CURRENT_SOURCE_DIR}/UnixJoystick.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UnixJoystick.h
	${CMAKE_CURRENT_SOURCE_DIR}/Platform.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlatformInput.cpp
	PARENT_SCOPE
)

set( PROCYON_INCLUDES
	${PROCYON_INCLUDES}
	${X11_INCLUDE_DIR}
	${X11_XCB_INCLUDE_DIR}
	${XCB_INCLUDE_DIRS}
	${UDEV_INCLUDE_DIR}
	${EVDEV_INCLUDE_DIR}
	PARENT_SCOPE
)

set( PROCYON_DEFINITIONS
	${PROCYON_DEFINITIONS}
	${LIBXCB_DEFINITIONS}
	${X11_XCB_DEFINITIONS}
	PARENT_SCOPE
)

set( PROCYON_LIBS
	${PROCYON_LIBS}
	${X11_LIBRARIES}
	${X11_XCB_LIBRARIES}
	${XCB_LIBRARIES}
	${UDEV_LIBRARIES}
	${EVDEV_LIBRARIES}
	PARENT_SCOPE
)
