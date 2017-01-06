# CMakeLists.txt for Procyon_Collision
add_library(Procyon_Collision STATIC
	World.cpp
)

target_link_libraries(Procyon_Collision
	Procyon_Common
	Procyon_Graphics
)

target_include_directories(Procyon_Collision PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Collision PARENT_SCOPE )
