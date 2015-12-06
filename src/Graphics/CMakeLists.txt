# CMakeLists.txt for Platformlib
cmake_minimum_required( VERSION 2.6 )

# OpenGL Library
find_package( OpenGL REQUIRED )

find_package( GLEW REQUIRED )

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
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/GL
)

target_link_libraries(Procyon_Graphics
	Procyon_Common
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Graphics PARENT_SCOPE )
