# Logog Static Library
# https://github.com/johnwbyrd/logog.git
if ( PROCYON_VS )
	set( GLFW_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}glfw-3.2/include" )
	set( GLFW_LIBRARY "${PROCYON_THIRDPARTY_ROOT}glfw-3.2/lib/vs110/x64/glfw3.lib")
else()
	find_package( GLFW REQUIRED )
endif()


set( PROCYON_SRCS
	${PROCYON_SRCS}
	${CMAKE_CURRENT_SOURCE_DIR}/Win32Window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Win32Window.h
	${CMAKE_CURRENT_SOURCE_DIR}/Win32GLContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Win32GLContext.h
	${CMAKE_CURRENT_SOURCE_DIR}/Platform.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlatformInput.cpp
	PARENT_SCOPE
)

set( PROCYON_INCLUDES
	${PROCYON_INCLUDES}
	${GLFW_INCLUDE_DIRS}
	PARENT_SCOPE
)

set( PROCYON_LIBS
	${PROCYON_LIBS}
	${GLFW_LIBRARY}
	PARENT_SCOPE
)
