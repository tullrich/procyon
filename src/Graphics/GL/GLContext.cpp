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

#include "GLContext.h"

namespace Procyon {
namespace GL {

	static void DebugCB(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam)
	{

	    PROCYON_ERROR("GL", "GL Error: %s ", message );
	}

	void GLContextBase::SharedInit()
	{
		static bool sGlewInitialized = false;
	    if( !sGlewInitialized )
	    {
	        InitGlew();
	        sGlewInitialized = true;
	    }

	    //TODO: Check to see if this debug extension is even available.
	    glEnable(GL_DEBUG_OUTPUT);
	    glDebugMessageCallback(DebugCB, NULL);
	}

	void GLContextBase::InitGlew()
	{
	    glewExperimental = GL_TRUE;

	    GLenum err = glewInit();
	    if( err != GLEW_OK )
	    {
	        throw std::runtime_error( 
	            reinterpret_cast< const char* >( glewGetErrorString( err ) ) );
	    }
	}

} /* namespace GL */
} /* namespace Procyon */
