# CMakeLists.txt for Procyon Editor
cmake_minimum_required( VERSION 2.6 )

# Box2D Library
find_package( Box2D REQUIRED )

add_executable(Box2dWorld 
    Box2dWorld.cpp
)

target_include_directories(Box2dWorld PUBLIC
    ${BOX2D_INCLUDE_DIRS}
)

target_link_libraries(Box2dWorld 
    Procyon_lib
    ${BOX2D_LIBRARY}
)