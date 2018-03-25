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
#include "GLGeometry.h"
#include "GLBuffer.h"
#include "GLProgram.h"

namespace Procyon {

namespace GL {

	GLGeometry::GLGeometry()
		: mIndices( NULL )
		, mIndexCount( 0 )
		, mPrimitiveMode( GL_TRIANGLES )
	{
        glGenVertexArrays( 1, &mVaoId );
	}

	GLGeometry::~GLGeometry()
	{
		glDeleteVertexArrays( 1, &mVaoId );
	}

	void GLGeometry::Bind( const GLProgram* program ) const
	{
        glBindVertexArray( mVaoId );

        if ( mIndices )
        	mIndices->Bind( GL_ELEMENT_ARRAY_BUFFER );

        for ( auto attribute : mAttributes )
        {
        	GLint attribLoc = program->GetAttributeLocation( attribute.name );

        	if ( attribLoc != -1 )
        	{
	        	attribute.buffer->Bind( GL_ARRAY_BUFFER );

	        	glVertexAttribPointer(attribLoc, attribute.components, attribute.type
	        		, attribute.normalize, attribute.stride
	        		, reinterpret_cast<const void*>( attribute.offset ) );

	        	glEnableVertexAttribArray(attribLoc);
        	}
        }
	}

	void GLGeometry::Unbind()
	{
        glBindVertexArray( 0 );
	}

	void GLGeometry::SetIndexBuffer( const GLBufferPtr& buf, int indexCount
		, GLenum primitive /*= GL_TRIANGLES */ )
	{
		mIndices 		= buf;
		mIndexCount 	= indexCount;
		mPrimitiveMode  = primitive;
	}

	GLBufferPtr GLGeometry::GetAttribute( const std::string& name )
	{
		auto search = std::find_if( mAttributes.begin(), mAttributes.end()
			, [&name]( const Attribute& a ) { return a.name == name; } );

		if ( search == mAttributes.end() )
		{
			return GLBufferPtr();
		}

		return search->buffer;
	}

	void GLGeometry::AddAttribute( const std::string& name, GLBufferPtr& buf, int components
		, int stride /*= 0*/, int offset /*= 0*/
		, GLenum type /*= GL_FLOAT*/, bool normalize /*= false*/ )
	{
		RemoveAttribute( name );

        Attribute attrib;
        attrib.buffer 		= buf;
        attrib.name 		= name;
        attrib.stride		= stride;
        attrib.offset 		= offset;
        attrib.components 	= components;
        attrib.type 		= type;
        attrib.normalize 	= normalize;

        mAttributes.push_back( attrib );
	}

	void GLGeometry::RemoveAttribute( const std::string& name )
	{
		mAttributes.erase( std::remove_if( mAttributes.begin(), mAttributes.end()
			, [&name]( const Attribute& a ) { return a.name == name; } ), mAttributes.end() );
	}

	void GLGeometry::Draw() const
	{
		if ( mIndexCount > 0 )
		{
			// TODO: Move this to the index buffer where you can dynamically define the harcoded
			// values below.
        	glDrawElements( mPrimitiveMode, mIndexCount, GL_UNSIGNED_SHORT, 0 );
		}
	}

} /* namespace GL */

} /* namespace Procyon */