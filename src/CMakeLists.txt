# Logog Static Library
# https://github.com/johnwbyrd/logog.git
if ( PROCYON_VS )
	set( LOGOG_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}logog/include" )
	set( LOGOG_LIBRARY "optimized;${PROCYON_THIRDPARTY_ROOT}logog/lib/vs110/x64/Release/logog.lib;debug;${PROCYON_THIRDPARTY_ROOT}logog/lib/vs110/x64/Debug/logogd.lib")
else()
	find_package( Logog REQUIRED )
endif()

# GLM Math Library
if ( PROCYON_VS )
	set( GLM_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}glm/include" )
else()
	find_package( GLM REQUIRED )
endif()

add_subdirectory( Graphics/ )
add_subdirectory( Platform/ )
add_subdirectory( Collision/ )
add_subdirectory( Audio/ )

set( PROCYON_SRCS
	${PROCYON_SRCS}
	${CMAKE_CURRENT_SOURCE_DIR}/MainLoop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MainLoop.h
	${CMAKE_CURRENT_SOURCE_DIR}/Console.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Console.h
	${CMAKE_CURRENT_SOURCE_DIR}/ProcyonCommon.h
	${CMAKE_CURRENT_SOURCE_DIR}/Macros.h
	${CMAKE_CURRENT_SOURCE_DIR}/Rect.h
	${CMAKE_CURRENT_SOURCE_DIR}/Aabb.h
	${CMAKE_CURRENT_SOURCE_DIR}/Logging.h
	${CMAKE_CURRENT_SOURCE_DIR}/Image.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Image.h
	${CMAKE_CURRENT_SOURCE_DIR}/Transformable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Transformable.h
	PARENT_SCOPE
)

set( PROCYON_INCLUDES
	${PROCYON_INCLUDES}
	${LOGOG_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${STB_INCLUDE_DIR}
	PARENT_SCOPE
)

set( PROCYON_LIBS
	${PROCYON_LIBS}
	${LOGOG_LIBRARY}
	${STB_LIBRARIES}
	PARENT_SCOPE
)

set( PROCYON_DEFINITIONS
	${PROCYON_DEFINITIONS}
    GLM_ENABLE_EXPERIMENTAL
	PARENT_SCOPE
)
