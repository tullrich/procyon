/*
===========================================================================

Procyon, a 2D game.

Copyright (C) 2015 Tim Ullrich.

This file is part of Procyon.

Procyon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Procyon is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Procyon.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/
#ifndef _X11_PLATFORM_H
#define _X11_PLATFORM_H

#include "ProcyonCommon.h"

#include <X11/Xlib.h>
#include <xcb/xcb.h>

/*
* X11 platform specific global state and functionality
*/

namespace Procyon {
namespace Unix {

    // The singleton x11 display open for the duration of the process
    extern Display* gDisplay;

    // The singleton xcb connection open for the duration of the process
    extern xcb_connection_t* gConnection;

} /* namespace Unix */
} /* namespace Procyon */

#endif /* _X11_PLATFORM_H */
