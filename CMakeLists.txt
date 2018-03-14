# CMakeLists.txt for Procyon
cmake_minimum_required( VERSION 2.6 )

# turn on with 'cmake -Dtest=ON'
option(test "Build all tests." OFF)
option(examples "Build the examples." ON)
option(editor "Build the editor." ON)
set(RenderInteface "GL" CACHE STRING "GL or DirectX")

# start proj
project( Procyon )

set( PROCYON_WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets )

# Platform determination
if ( UNIX )
	set( PROCYON_LINUX true )
elseif( MSVC OR MSVC_IDE )

  if( MSVC_VERSION LESS 1700 )       # VC10-/VS2010-
    message(FATAL_ERROR "The project requires C++11 features. "
      "You need at least Visual Studio 11 (Microsoft Visual Studio 2012), "
      "with Microsoft Visual C++ Compiler Nov 2012 CTP (v120_CTP_Nov2012).")
  elseif( MSVC_VERSION EQUAL 1700 )  # VC11/VS2012
    message( "VC11: use Microsoft Visual Studio 2012 "
      "with Microsoft Visual C++ Compiler Nov 2012 CTP (v120_CTP_Nov2012)" )
    set(CMAKE_GENERATOR_TOOLSET "v120_CTP_Nov2012" CACHE STRING "Platform Toolset" FORCE)
  endif()

  file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user"
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>     \
    <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
        <LocalDebuggerWorkingDirectory>$(PROCYON_WORKING_DIR)</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
        <LocalDebuggerWorkingDirectory>$(PROCYON_WORKING_DIR)</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
    </Project>")

  # variadic templates support required
  set( PROCYON_VS true )
else( )
  message(FATAL_ERROR "Unsupported platform")
endif ()

# Render backend
if ( RenderInteface STREQUAL "GL" )
	set( PROCYON_GL true )
elseif ( RenderInteface STREQUAL "DirectX" )
	if ( PROCYON_LINUX )
		message( FATAL_ERROR "DirectX unsupported on linux." )
	endif ()
	set( PROCYON_DIRECTX true )
else ()
  message( FATAL_ERROR "Unknown render interface: ${RenderInteface}" )
endif ()

# define the path of our additional CMake modules
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( PROCYON_THIRDPARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/" )
set( PROCYON_SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/" )

add_subdirectory( lib/stb )
add_subdirectory( lib/vaserenderer )

# Procyon Lib
set( PROCYON_SRCS )
set( PROCYON_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( PROCYON_LIBS )
set( PROCYON_DEFINITIONS NOMINMAX _USE_MATH_DEFINES )
add_subdirectory( src/ )

# Procyon lib
add_library( Procyon STATIC ${PROCYON_SRCS} )
target_include_directories( Procyon PUBLIC ${PROCYON_INCLUDES} )
target_link_libraries( Procyon PUBLIC ${PROCYON_LIBS} )
target_compile_definitions( Procyon PUBLIC ${PROCYON_DEFINITIONS} )

if ( NOT PROCYON_VS )
	target_compile_options( Procyon PUBLIC
		-std=c++0x
		-g
	)
endif()

# optional tests
if ( test )
	add_subdirectory( tests )
endif()

# optional examples
if ( examples )
	add_subdirectory( examples/Sandbox )
	#add_subdirectory( examples/Box2dWorld )
endif()

# optional editor
if ( editor )
	add_subdirectory( editor )
endif()
