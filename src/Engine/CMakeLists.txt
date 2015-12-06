# CMakeLists.txt for Platformlib
cmake_minimum_required( VERSION 2.6 )

add_library(Procyon_Engine STATIC
	MainLoop.cpp
	Console.cpp
)

target_link_libraries(Procyon_Engine
	Procyon_Common
	Procyon_Graphics
	Procyon_Platform
	Procyon_Collision
	Procyon_Audio
)

target_include_directories(Procyon_Engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Engine PARENT_SCOPE )
