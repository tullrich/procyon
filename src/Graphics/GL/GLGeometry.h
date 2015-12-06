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
#ifndef _GL_GEOMETRY_H
#define _GL_GEOMETRY_H

#include "ProcyonGL.h"
#include "GLBuffer.h"

namespace Procyon {

namespace GL {

	class GLProgram;

	class GLGeometry
	{

		struct Attribute
		{
			std::string name;
			int 		components;
			GLenum		type;
			bool		normalize;
			int			stride;
			int			offset;
			GLBufferPtr	buffer;		

			Attribute()
				: components( 0 )
				, type( GL_BYTE )
				, normalize( false )
				, stride( 0 )
				, offset( 0 )
				, buffer( NULL )
			{
			}
		};

	public:
						GLGeometry();
						~GLGeometry();

		void 			Bind( const GLProgram* program ) const;
		void 			Unbind();

		void 			SetIndexBuffer( const GLBufferPtr& buf
							, int indexCount, GLenum primitive = GL_TRIANGLES );

		GLBufferPtr 	GetAttribute( const std::string& name );
		void 			AddAttribute( const std::string& name, GLBufferPtr& buf, int components
							, int stride = 0, int offset = 0
							, GLenum type = GL_FLOAT, bool normalize = false );
		void 			RemoveAttribute( const std::string& name );

		virtual void 	Draw() const;

    	typedef std::vector<Attribute> AttributeArray;

    	GLuint 			mVaoId;
    	AttributeArray 	mAttributes;
    	GLBufferPtr		mIndices;
    	int 			mIndexCount;
    	GLenum 			mPrimitiveMode;
	};

} /* namespace GL */

} /* namespace Procyon */

#endif /* _GL_GEOMETRY_H */