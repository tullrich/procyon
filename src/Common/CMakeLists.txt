# CMakeLists.txt for Procyon_Common

# Logog Static Library
# https://github.com/johnwbyrd/logog.git
if ( PROCYON_VS )
	set( LOGOG_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}logog/include" )
	set( LOGOG_LIBRARY "optimized;${PROCYON_THIRDPARTY_ROOT}logog/lib/vs110/x64/Release/logog.lib;debug;${PROCYON_THIRDPARTY_ROOT}logog/lib/vs110/x64/Debug/logogd.lib")
else()
	find_package( Logog REQUIRED )
endif()

# GLM Math Library
if ( PROCYON_VS )
	set( GLM_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}glm/include" )
else()
	find_package( GLM REQUIRED )
endif()

add_library(Procyon_Common STATIC
	Reflection.cpp
	Ioc.cpp
	Image.cpp
	Transformable.cpp
)

if ( NOT PROCYON_VS )
	target_compile_options(Procyon_Common PUBLIC
		-std=c++0x
		-g
	)
endif()

target_include_directories(Procyon_Common PUBLIC
	${LOGOG_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${STB_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Procyon_Common
	${LOGOG_LIBRARY}
	${STB_LIBRARIES}
	${PROCYON_LINKER_INPUT}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Common PARENT_SCOPE )
