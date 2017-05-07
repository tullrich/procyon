# TinyXML Library
if ( PROCYON_VS )
	set( TINYXML2_INCLUDE_DIR "${PROCYON_THIRDPARTY_ROOT}tinyxml2/include" )
	set( TINYXML2_LIBRARIES "optimized;${PROCYON_THIRDPARTY_ROOT}tinyxml2/lib/vs110/x64/Release/tinyxml2.lib;debug;${PROCYON_THIRDPARTY_ROOT}tinyxml2/lib/vs110/x64/Debug/tinyxml2.lib")
else()
	find_package( TinyXML2 REQUIRED )
endif()

add_executable(Sandbox
	main.cpp
	Sandbox.cpp
	Player.cpp
	SandboxAssets.cpp
	XmlMap.cpp
	${CMAKE_SOURCE_DIR}/editor/Grid.cpp
)

include_directories(
	${TINYXML2_INCLUDE_DIR}
)

target_link_libraries(Sandbox
	${PROCYON_LINKER_INPUT}
	${TINYXML2_LIBRARIES}
)
