# CMakeLists.txt for audio module
cmake_minimum_required( VERSION 2.6 )

# OpenAl audio library
if ( PROCYON_VS )
	set( OPENAL_INCLUDE_DIR "${PROCYON_THIRDPARTY_ROOT}openal-1.1/include" )
	set( OPENAL_LIBRARY "${PROCYON_THIRDPARTY_ROOT}openal-1.1/lib/vs110/x64/OpenAL32.lib" )
else()
	find_package( OpenAL REQUIRED )
endif()

# SndFile audio file import library
if ( PROCYON_VS )
	set( SNDFILE_INCLUDE_DIRS "${PROCYON_THIRDPARTY_ROOT}libsndfile/include" )
	set( SNDFILE_LIBRARIES "${PROCYON_THIRDPARTY_ROOT}libsndfile/lib/vs110/x64/libsndfile-1.lib" )
else()
	find_package( SndFile REQUIRED )
endif()

add_library(Procyon_Audio STATIC
	AudioDevice.cpp
	SoundFile.cpp
	Sound.cpp
	SoundBuffer.cpp
)

target_link_libraries(Procyon_Audio
	Procyon_Common
	${OPENAL_LIBRARY}
	${SNDFILE_LIBRARIES}
)

target_include_directories(Procyon_Audio PUBLIC
	${OPENAL_INCLUDE_DIR}
	${SNDFILE_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set( PROCYON_LINKER_INPUT ${PROCYON_LINKER_INPUT} Procyon_Audio PARENT_SCOPE )
