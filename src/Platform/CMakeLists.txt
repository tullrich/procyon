set( PROCYON_PLATFORM_INCLUDE_DIRS )
set( PROCYON_PLATFORM_DEFINITIONS )
set( PROCYON_PLATFORM_LIBRARIES )
set( PROCYON_PLATFORM_SOURCES )
if ( PROCYON_LINUX )
	add_subdirectory( Unix/ )
else()
	add_subdirectory( Win32/ )
endif()

add_library(Procyon_Platform STATIC
	Window.cpp
	Joystick.cpp
	${PROCYON_PLATFORM_SOURCES}
)

target_include_directories(Procyon_Platform PUBLIC
	${PROCYON_PLATFORM_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Procyon_Platform PUBLIC
	${PROCYON_PLATFORM_DEFINITIONS}
)

target_link_libraries(Procyon_Platform
	Procyon_Common
	Procyon_Graphics
	${PROCYON_PLATFORM_LIBRARIES}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Platform PARENT_SCOPE )
