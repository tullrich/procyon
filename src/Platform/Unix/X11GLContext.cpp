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

#include "X11GLContext.h"

namespace Procyon {
namespace Unix {

X11GLContext::X11GLContext( X11Window *window )
	: mWindow( window )
{
    if( !CreateX11Context() )
        throw std::runtime_error("X11GLContext");

    SharedInit();
}

X11GLContext::~X11GLContext()
{
    Display* display = mWindow->GetXDisplay();

    glXDestroyWindow( display, mGLXWindow );
    glXDestroyContext( display, mContext );
}

bool X11GLContext::CreateX11Context()
{
    int attribList[] = { 
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, 
        GLX_SAMPLE_BUFFERS  , 1,            // <-- MSAA
        GLX_SAMPLES         , 4,
        0 
    };

    Display* display = mWindow->GetXDisplay();
    xcb_window_t window = mWindow->GetXWindow();

	GLXFBConfig *fb_configs = 0;
	int num_fb_configs = 0;
	fb_configs = glXChooseFBConfig( display, DefaultScreen(display), attribList, &num_fb_configs );
	if( !fb_configs || num_fb_configs == 0 )
	{
		PROCYON_ERROR( "X11", "Error querying framebuffer configuration." );
		return false;
	}

    mFBConfig = fb_configs[0];

    mContext = glXCreateNewContext(display, mFBConfig, GLX_RGBA_TYPE, 0, true);
    if( !mContext )
    {
		PROCYON_ERROR( "X11", "Unable to create GLXContext." );
        return false;
    }

	mGLXWindow = glXCreateWindow(
	    display,
	    mFBConfig,
	    window,
	    0
	    );
	if( !mGLXWindow )
    {
		PROCYON_ERROR( "X11", "Unable to create GLX Drawable." );
        return false;
    }

    /* make OpenGL context current */
    if( !glXMakeCurrent( display, mGLXWindow, mContext) )
    {
    	glXDestroyWindow( display, mGLXWindow );
        glXDestroyContext( display, mContext );
		PROCYON_ERROR( "X11", "Unable to make GLXContext current." );
        return false;
    }


    XVisualInfo *vi = glXGetVisualFromFBConfig( display, mFBConfig );
    if ( vi )
    {
      int samp_buf, samples;
      glXGetFBConfigAttrib( display, mFBConfig, GLX_SAMPLE_BUFFERS, &samp_buf );
      glXGetFBConfigAttrib( display, mFBConfig, GLX_SAMPLES       , &samples  );
 
      PROCYON_DEBUG( "X11", "Matching fbconfig visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
              " SAMPLES = %d\n", vi->visualid, samp_buf, samples );
    }
    else
    {
        PROCYON_WARN( "X11", "Failed to retrieve a framebuffer config\n" );
    }

    return true;
}

void X11GLContext::MakeCurrent()
{
    Display* display = mWindow->GetXDisplay();
    glXMakeCurrent( display, mGLXWindow, mContext );
}

void X11GLContext::SwapBuffers()
{
    Display* display = mWindow->GetXDisplay();
    glXSwapBuffers( display, mGLXWindow );
}

} /* namespace Unix */
} /* namespace Procyon */
