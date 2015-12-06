# CMakeLists.txt for Procyon Editor
cmake_minimum_required( VERSION 2.6 )

add_executable(Sandbox 
	main.cpp
	Sandbox.cpp 
	Player.cpp
	SandboxAssets.cpp
	${CMAKE_SOURCE_DIR}/editor/Grid.cpp
)

target_link_libraries(Sandbox 
	Procyon_lib
)