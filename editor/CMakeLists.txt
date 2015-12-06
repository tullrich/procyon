# CMakeLists.txt for Procyon Editor
cmake_minimum_required( VERSION 2.6 )
	
# Find QT5 (Widgets includes QT5 Base and other modules).
find_package( Qt5Widgets REQUIRED )
find_package( Qt5X11Extras REQUIRED )
find_package( Qt5OpenGL REQUIRED )

# Finds all Q_OBJECT classes and generates + links Editor_automoc.cpp in the binary dir.
set( CMAKE_AUTOMOC ON )

# Finds all <name>.ui files and generates a ui_<name>.h header in the binary dir.
set( CMAKE_AUTOUIC ON )

# Allows the build to find the above generated files in the binary dir.
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# Adds preprocessor definitions.
add_definitions( ${Qt5Widgets_DEFINITIONS} )

# Add QT5 includes (also includes other modules)
include_directories( 
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5X11Extras_INCLUDE_DIRS}
	)

# Create the Editor.exe
add_executable(Editor 
	editor.cpp 
	editor_main.cpp
	ProcyonCanvas.cpp
	ProcyonQtUtil.cpp
	Grid.cpp
	LogOutputTarget.cpp
	EditorAssets.cpp
	)

# Link against Qt5 and Procyon targets.
target_link_libraries(Editor
	Procyon_lib 
	${Qt5Widgets_LIBRARIES} 
	${Qt5X11Extras_LIBRARIES}
	${Qt5OpenGL_LIBRARIES}
	)