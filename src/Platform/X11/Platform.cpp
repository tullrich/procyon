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
#include "Platform/Platform.h"
#include "X11Platform.h"

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

namespace Procyon {

    namespace Unix {
        Display* gDisplay = nullptr;
        xcb_connection_t* gConnection = nullptr;
		xcb_screen_t* gScreen = nullptr;
    } /* namespace Unix */

	/* static */ void Platform::Init()
	{
        using namespace Unix;

    	// Open Xlib Display
    	gDisplay = XOpenDisplay( 0 );
    	if( !gDisplay )
    	{
    		PROCYON_ERROR( "X11", "Unable to open display." );
        	throw std::runtime_error("X11Window");
    	}

    	// Open XCB Connection
        gConnection = XGetXCBConnection( gDisplay );
      	if( !gConnection )
        {
        	Destroy();
    		PROCYON_ERROR( "X11", "Unable to open xcb connection" );
        	throw std::runtime_error("X11Window");
        }
        XSetEventQueueOwner( gDisplay, XCBOwnsEventQueue );

        // Grab the first screen
        gScreen = xcb_setup_roots_iterator( xcb_get_setup( gConnection ) ).data;
	}

	/* static */ void Platform::Destroy()
	{
        using namespace Unix;

    	xcb_flush( gConnection );

    	if( gDisplay )
    	{
       		XCloseDisplay( gDisplay );
       		gConnection = NULL; // connection is automatically shutdown
       		gDisplay = NULL;
            gScreen = NULL;
    	}
	}
} /* namespace Procyon */
