# CMakeLists.txt for STB
SET(QPROPERTYBROWSER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/src PARENT_SCOPE)

include_directories( ${QPROPERTYBROWSER_INCLUDE_DIR}/src )

add_library(qpropertybrowser_lib
  src/qtbuttonpropertybrowser.cpp
  src/qteditorfactory.cpp
  src/qtgroupboxpropertybrowser.cpp
  src/qtpropertybrowser.cpp
  src/qtpropertybrowserutils.cpp
  src/qtpropertymanager.cpp
  src/qttreepropertybrowser.cpp
  src/qtvariantproperty.cpp)

set_target_properties(qpropertybrowser_lib PROPERTIES COMPILE_FLAGS "-fPIC" )

SET(QPROPERTYBROWSER_LIBRARIES qpropertybrowser_lib PARENT_SCOPE)

mark_as_advanced(QPROPERTYBROWSER_INCLUDE_DIR)
mark_as_advanced(QPROPERTYBROWSER_LIBRARIES)
