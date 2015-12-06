# CMakeLists.txt for Platformlib
cmake_minimum_required( VERSION 2.6 )

# Logog Static Library
# https://github.com/johnwbyrd/logog.git
find_package( Logog REQUIRED )

# GLM Math Library
find_package( GLM REQUIRED )

# TinyXML Library
find_package( TinyXML REQUIRED )

add_library(Procyon_Common STATIC
	Reflection.cpp 
	Ioc.cpp 
	Image.cpp
	Transformable.cpp
)

target_compile_options(Procyon_Common PUBLIC 
	-std=c++0x
	-g
)

target_include_directories(Procyon_Common PUBLIC
	${LOGOG_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${STB_INCLUDE_DIR}
	${TinyXML_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Procyon_Common
	${LOGOG_LIBRARY}
	${TinyXML_LIBRARIES}
	${PROCYON_LINKER_INPUT}
	${STB_LIBRARIES}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Common PARENT_SCOPE )
