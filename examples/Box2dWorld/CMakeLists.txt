# Box2D Library
find_package( Box2D REQUIRED )

add_executable(Box2dWorld
    Box2dWorld.cpp
)

target_include_directories(Box2dWorld PUBLIC
    ${BOX2D_INCLUDE_DIRS}
)

target_link_libraries(Box2dWorld
    Procyon
    ${BOX2D_LIBRARY}
)
