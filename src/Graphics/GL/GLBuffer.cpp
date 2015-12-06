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
#include "GLBuffer.h"

namespace Procyon {

namespace GL {

	GLBuffer::GLBuffer()
		: mAllocated( false )
	{
		glGenBuffers( 1, &mBufferId );
	}


	GLBuffer::GLBuffer( GLsizeiptr size, const void* data
		, GLenum usage /* = GL_STATIC_DRAW */ )
		: mAllocated( false )
	{
		glGenBuffers( 1, &mBufferId );
		SetData( size, data, usage );
	}

	GLBuffer::~GLBuffer()
	{
		glDeleteBuffers( 1, &mBufferId );
	}

	void GLBuffer::SetData( GLsizeiptr size, const void* data
		, GLenum usage /* = GL_STATIC_DRAW */ )
	{
	    glBindBuffer( GL_ARRAY_BUFFER, mBufferId );

	    if ( !mAllocated )
	    {
	    	glBufferData( GL_ARRAY_BUFFER, size, data, usage );
	    }
	    else
	    {
	    	//void* map = glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
	    	//memcpy( map, data, size );
	    	//glUnmapBuffer( GL_ARRAY_BUFFER );
	    	//glBufferData( GL_ARRAY_BUFFER, size, 0, usage );
	    	glBufferSubData( GL_ARRAY_BUFFER, 0, size, data );
	    }

	    mAllocated = true;
	}

	void GLBuffer::Bind( GLenum target )
	{
	    glBindBuffer( target, mBufferId );
	}

} /* namespace GL */

} /* namespace Procyon */