# CMakeLists.txt GL source files
find_package( OpenGL REQUIRED )

if ( PROCYON_VS )
	set( GLEW_INCLUDE_DIR "${PROCYON_THIRDPARTY_ROOT}glew-1.13.0/include" )
	set( GLEW_LIBRARIES "${PROCYON_THIRDPARTY_ROOT}glew-1.13.0/lib/Release/x64/glew32s.lib" )
else()
	find_package( GLEW REQUIRED )
endif()

set( PROCYON_SRCS
	${PROCYON_SRCS}
	${CMAKE_CURRENT_SOURCE_DIR}/GLTexture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLShader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLProgram.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLBuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLGeometry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLMaterial.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GLRenderCore.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ProcyonVaser.cpp
	PARENT_SCOPE
)

set( PROCYON_INCLUDES
	${PROCYON_INCLUDES}
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	PARENT_SCOPE
)

set( PROCYON_LIBS
	${PROCYON_LIBS}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	PARENT_SCOPE
)

set( PROCYON_DEFINITIONS
	${PROCYON_DEFINITIONS}
	-DGLEW_STATIC
	PARENT_SCOPE
)
