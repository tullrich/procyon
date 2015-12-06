# CMakeLists.txt for audio module
cmake_minimum_required( VERSION 2.6 )

find_package( OpenAL REQUIRED )
find_package( SndFile REQUIRED )

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
