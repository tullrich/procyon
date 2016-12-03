# CMakeLists.txt for Procyon
cmake_minimum_required( VERSION 2.6 )

# turn on with 'cmake -Dtest=ON'
option(test "Build all tests." OFF)
option(examples "Build the examples." ON)
option(editor "Build the editor." ON)

# start proj
project( Procyon )

# Platform determination
if ( UNIX )
	set( PROCYON_LINUX true )
endif ( UNIX )

# define the path of our additional CMake modules
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory( lib/stb )
add_subdirectory( lib/vaserenderer )

# Modules
set(PROCYON_LINKER_INPUT)
add_subdirectory( src/Common )
add_subdirectory( src/Graphics )
add_subdirectory( src/Platform )
add_subdirectory( src/Collision )
add_subdirectory( src/Engine )
add_subdirectory( src/Audio )

# Procyon lib
add_library(Procyon_lib STATIC)

target_link_libraries(Procyon_lib
	${PROCYON_LINKER_INPUT}
)

# optional tests
if ( test )
	add_subdirectory( tests )
endif()

# optional examples
if ( examples )
	add_subdirectory( examples/Sandbox )
	add_subdirectory( examples/Box2dWorld )
endif()

# optional editor
if ( editor )
	add_subdirectory( editor )
endif()
