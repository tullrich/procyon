# CMakeLists.txt for Procyon Editor
cmake_minimum_required( VERSION 2.6 )

# TinyXML Library
find_package( TinyXML REQUIRED )

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