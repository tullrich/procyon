# CMakeLists.txt for 2DGame
cmake_minimum_required( VERSION 2.6 )

# start proj.
project( 2DGame )

# define the path of our additional CMake modules
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )

# Logog Static Library
# https://github.com/johnwbyrd/logog.git
find_package( Logog REQUIRED )
include_directories( ${LOGOG_INCLUDE_DIRS} )

# Simple Fast Media Library (SFML)
# Possible link targets system, window, graphics, network, audio, main
find_package( SFML REQUIRED COMPONENTS system window graphics network audio )
include_directories( ${SFML_INCLUDE_DIR} )

# GLM Math Library
find_package( GLM REQUIRED )
include_directories( ${GLM_INCLUDE_DIRS} )

# TinyXML Library
find_package( TinyXML REQUIRED )
include_directories( ${TinyXML_INCLUDE_DIRS} )

# Box2D Library
find_package( Box2D REQUIRED )
include_directories( ${BOX2D_INCLUDE_DIRS} )

# 2DGame exe
add_executable( 2DGame src/main.cpp src/reflection.cpp )
target_link_libraries( 2DGame ${LOGOG_LIBRARY} ${SFML_LIBRARIES} ${TinyXML_LIBRARIES} ${BOX2D_LIBRARIES} )