# CMakeLists.txt for Procyon_Graphics
find_package( OpenGL REQUIRED )

if ( PROCYON_VS )
	set( FREETYPE_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}freetype-2.6.5/include" )
	set( FREETYPE_LIBRARIES "${PROCYON_THIRDPARTY_ROOT}freetype-2.6.5/lib/vs110/x64/freetype.lib" )
else()
	find_package( Freetype REQUIRED )
endif()

set( PROCYON_GRAPHICS_SRC_FILES )
set( PROCYON_GRAPHICS_INCLUDE_DIRS )
set( PROCYON_GRAPHICS_LIBRARIES )
set( PROCYON_GRAPHICS_DEFINITIONS )

if ( PROCYON_GL )
	add_subdirectory( GL/ )
elseif ( PROCYON_DIRECTX )
	add_subdirectory( DirectX/ )
endif ()

add_library(Procyon_Graphics STATIC
	Camera.cpp
	Sprite.cpp
	Shape.cpp
	FontFace.cpp
	Renderer.cpp
	Text.cpp
	${PROCYON_GRAPHICS_SRC_FILES}
)

target_compile_definitions(Procyon_Graphics PUBLIC
	${PROCYON_GRAPHICS_DEFINITIONS}
)

target_include_directories(Procyon_Graphics PUBLIC
	${FREETYPE_INCLUDE_DIRS}
	${VASERENDERER_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${PROCYON_GRAPHICS_INCLUDE_DIRS}
)

target_link_libraries(Procyon_Graphics
	Procyon_Common
	${FREETYPE_LIBRARIES}
	${PROCYON_GRAPHICS_LIBRARIES}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Graphics PARENT_SCOPE )
