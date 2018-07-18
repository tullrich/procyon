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

    static const char* GLErrorSourceToString( GLenum source )
    {
        switch ( source )
        {
            case GL_DEBUG_SOURCE_API: return "GL_DEBUG_SOURCE_API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "GL_DEBUG_SOURCE_THIRD_PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "GL_DEBUG_SOURCE_APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: // intentional fallthrough
            default: return "GL_DEBUG_SOURCE_OTHER";
        }
    }

    static const char* GLErrorTypeToString( GLenum type )
    {
        switch ( type )
        {
            case GL_DEBUG_TYPE_ERROR: return "GL_DEBUG_TYPE_ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "GL_DEBUG_TYPE_PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "GL_DEBUG_TYPE_PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "GL_DEBUG_TYPE_MARKER";
            case GL_DEBUG_TYPE_PUSH_GROUP: return "GL_DEBUG_TYPE_PUSH_GROUP";
            case GL_DEBUG_TYPE_POP_GROUP: return "GL_DEBUG_TYPE_POP_GROUP";
            case GL_DEBUG_TYPE_OTHER: // intentional fallthrough
            default: return "GL_DEBUG_TYPE_OTHER";
        }
    }

    static const char* GLErrorSeverityToString( GLenum severity )
    {
        switch ( severity )
        {
            case GL_DEBUG_SEVERITY_HIGH: return "GL_DEBUG_SEVERITY_HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM: return "GL_DEBUG_SEVERITY_MEDIUM";
            case GL_DEBUG_SEVERITY_LOW: return "GL_DEBUG_SEVERITY_LOW";
            case GL_DEBUG_SEVERITY_NOTIFICATION: // intentional fallthrough
            default: return "GL_DEBUG_SEVERITY_NOTIFICATION";
        }
    }

	static void DebugCB(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam)
	{
        if ( severity != GL_DEBUG_SEVERITY_NOTIFICATION )
        {
    	    PROCYON_ERROR( "GL", "GL Error: \n\tSource:\t%s \n\tType:\t%s \n\tId:\t%i \n\tLevel:\t%s \n\tMsg:\t%s ",   \
                GLErrorSourceToString( source ),
                GLErrorTypeToString( type ),
                id,
                GLErrorSeverityToString( severity ),
                message );
        }
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
	    glEnable( GL_DEBUG_OUTPUT );
	    glDebugMessageCallback( DebugCB, NULL );
	}

	void GLContextBase::InitGlew()
	{
	    glewExperimental = GL_TRUE;

	    GLenum err = glewInit();
	    if( err != GLEW_OK )
	    {
            const GLubyte* msg = glewGetErrorString( err );
            PROCYON_ERROR( "GL", "Glew Error: %s ", msg );
	        throw std::runtime_error( "Terminal" );
	    }
	}

} /* namespace GL */
} /* namespace Procyon */
