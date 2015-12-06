# CMakeLists.txt for Unix PlatformLib implementation
cmake_minimum_required( VERSION 2.6 )

set( PROCYON_PLATFORM_SRC_FILES 
	${PROCYON_PLATFORM_SRC_FILES} 
	${CMAKE_CURRENT_SOURCE_DIR}/X11GLContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/X11Window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UnixJoystick.cpp
PARENT_SCOPE )
