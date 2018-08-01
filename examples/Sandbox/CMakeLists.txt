# TinyXML Library
if ( PROCYON_VS )
	set( TINYXML2_INCLUDE_DIR "${PROCYON_THIRDPARTY_ROOT}tinyxml2/include" )
	set( TINYXML2_LIBRARIES "optimized;${PROCYON_THIRDPARTY_ROOT}tinyxml2/lib/vs110/x64/Release/tinyxml2.lib;debug;${PROCYON_THIRDPARTY_ROOT}tinyxml2/lib/vs110/x64/Debug/tinyxml2.lib")
else()
	find_package( TinyXML2 REQUIRED )
endif()

add_executable( Sandbox
	main.cpp
	Sandbox.h
	Sandbox.cpp
	Player.h
	Player.cpp
	SandboxAssets.h
	SandboxAssets.cpp
	XmlMap.h
	XmlMap.cpp
	PolyLine.h
	PolyLine.cpp
	${CMAKE_SOURCE_DIR}/editor/Grid.cpp
)

target_include_directories( Sandbox PUBLIC
	${PROCYON_INCLUDES}
	${TINYXML2_INCLUDE_DIR}
)

target_link_libraries( Sandbox PUBLIC
	Procyon
	${TINYXML2_LIBRARIES}
)

target_compile_definitions( Sandbox PUBLIC
	${PROCYON_DEFINITIONS}
)
