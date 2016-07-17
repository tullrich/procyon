# CMakeLists.txt for Platformlib
cmake_minimum_required( VERSION 2.6 )

find_package( OpenGL REQUIRED )

if ( PROCYON_VS )
	set( GLEW_INCLUDE_DIR "${PROCYON_THIRDPARTY_ROOT}glew-1.13.0/include" )
	set( GLEW_LIBRARIES "${PROCYON_THIRDPARTY_ROOT}glew-1.13.0/lib/Release/x64/glew32s.lib" )
else()
	find_package( GLEW REQUIRED )
endif()

if ( PROCYON_VS )
	set( FREETYPE_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}freetype-2.6.5/include" )
	set( FREETYPE_LIBRARIES "${PROCYON_THIRDPARTY_ROOT}freetype-2.6.5/lib/vs110/x64/freetype.lib" )
else()
	find_package( Freetype REQUIRED )
endif()

set( PROCYON_GRAPHICS_SRC_FILES )
add_subdirectory( GL/ )

add_library(Procyon_Graphics STATIC
	RenderContext.cpp
	Camera.cpp
	FontRenderer.cpp
	Sprite.cpp
	SpriteRenderer.cpp
	Shape.cpp
	LineShape.cpp
	RectShape.cpp
	ShapeRenderer.cpp
	Quad.cpp
	FontFace.cpp
	Renderer.cpp
	RenderCore.cpp
	Text.cpp
	${PROCYON_GRAPHICS_SRC_FILES}
)

target_compile_definitions(Procyon_Graphics PUBLIC
	-DGLEW_STATIC
)

target_include_directories(Procyon_Graphics PUBLIC
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${VASERENDERER_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/GL
)

target_link_libraries(Procyon_Graphics
	Procyon_Common
	${FREETYPE_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Graphics PARENT_SCOPE )
