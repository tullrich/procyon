# CMakeLists.txt for vaserenderer
cmake_minimum_required( VERSION 2.6 )

SET(VASERENDERER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

include_directories(${VASERENDERER_INCLUDE})

mark_as_advanced(VASERENDERER_INCLUDE_DIR)
