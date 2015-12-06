# EVDEV_FOUND - system has a libevdev
# EVDEV_INCLUDE_DIR - where to find header files
# EVDEV_LIBRARIES - the libraries to link against evdev
#
# Adapted from FindUDev.cmake from Razor-qt for Procyon
# copyright (c) 2011 Petr Vanek <petr@scribus.info>
# Redistribution and use is allowed according to the terms of the BSD license.
#
FIND_PATH(
    EVDEV_INCLUDE_DIR
    NAMES libevdev.h libevdev-1.0
    PATH_SUFFIXES libevdev-1.0/libevdev/
    PATHS
        /usr/include/
        /usr/local/include
        ${EVDEV_PATH_INCLUDES}
)

FIND_LIBRARY(
    EVDEV_LIBRARIES
    NAMES evdev libevdev
    PATHS
        /usr/lib${LIB_SUFFIX}
        /usr/local/lib${LIB_SUFFIX}
        ${EVDEV_PATH_LIB}
)

IF (EVDEV_LIBRARIES AND EVDEV_INCLUDE_DIR)
    SET(EVDEV_FOUND "YES")
ENDIF (EVDEV_LIBRARIES AND EVDEV_INCLUDE_DIR)
