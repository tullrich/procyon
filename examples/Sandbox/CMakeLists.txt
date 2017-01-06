# TinyXML Library
if ( PROCYON_VS )
	set( TinyXML_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}tinyxml2/include" )
	set( TinyXML_LIBRARIES "optimized;${PROCYON_THIRDPARTY_ROOT}tinyxml2/lib/vs110/x64/Release/tinyxml2.lib;debug;${PROCYON_THIRDPARTY_ROOT}tinyxml2/lib/vs110/x64/Debug/tinyxml2.lib")
else()
	find_package( TinyXML REQUIRED )
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
	${TinyXML_INCLUDE_DIRS}
)

target_link_libraries(Sandbox
	Procyon_lib
	${TinyXML_LIBRARIES}
)
