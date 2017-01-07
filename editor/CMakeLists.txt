# CMakeLists.txt for Procyon Editor
# Fix to find cmake on Windows
set( CMAKE_PREFIX_PATH "D:/Qt/5.7/msvc2015_64" )

# Find QT5 (Widgets includes QT5 Base and other modules).
find_package( Qt5Widgets REQUIRED )
find_package( Qt5OpenGL REQUIRED )

# Disable automatic linking against qtmain.lib
cmake_policy( SET CMP0020 NEW )

if ( PROCYON_LINUX )
	find_package( Qt5X11Extras REQUIRED )
	set( PROCYON_PLATFORM_INCLUDE_DIRS ${Qt5X11Extras_INCLUDE_DIRS} )
	set( PROCYON_PLATFORM_LIBRARIES ${Qt5X11Extras_LIBRARIES} )
elseif( PROCYON_VS )
	set( PROCYON_PLATFORM_SOURCES
		resources/app.rc
		resources/app.ico )
endif()

# Finds all Q_OBJECT classes and generates + links Editor_automoc.cpp in the binary dir.
set( CMAKE_AUTOMOC ON )

# Finds all <name>.ui files and generates a ui_<name>.h header in the binary dir.
set( CMAKE_AUTOUIC ON )

# Finds specified .qrc files and runs the rcc tool to generate
set( CMAKE_AUTORCC ON )

# Allows the build to find the above generated files in the binary dir.
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# Adds preprocessor definitions.
add_definitions( ${Qt5Widgets_DEFINITIONS} )

# Add QT5 includes (also includes other modules)
include_directories(
	${Qt5Widgets_INCLUDE_DIRS}
	${PROCYON_PLATFORM_INCLUDE_DIRS}
	)

add_subdirectory( QPropertyBrowser )
include_directories( ${QPROPERTYBROWSER_INCLUDE_DIR} )

# Create the Editor.exe
add_executable(Editor WIN32
	main.cpp
	Editor.cpp
	ProcyonCanvas.cpp
	ProcyonQtUtil.cpp
	Grid.cpp
	LogOutputTarget.cpp
	EditorAssets.cpp
	MapDocument.cpp
	EditorSettings.cpp
	RecentFileList.cpp
	SceneObject.cpp
	PrefsDialog.cpp
	NewMapDialog.cpp
	editor_resources.qrc
	${PROCYON_PLATFORM_SOURCES}
)

# Link against Qt5 and Procyon targets.
target_link_libraries(Editor
	Procyon_lib
	${Qt5Widgets_LIBRARIES}
	${Qt5OpenGL_LIBRARIES}
	${QPROPERTYBROWSER_LIBRARIES}
	${PROCYON_PLATFORM_LIBRARIES}
)
