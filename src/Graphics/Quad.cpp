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
#include "Quad.h"

using namespace Procyon::GL;

namespace Procyon {

	static float sVerts[] = 
	{
	    //  Position  Texcoords
	    0.0f, 0.0f, 0.0f, 0.0f, // Top-left
	    1.0f, 0.0f, 1.0f, 0.0f, // Top-right
	    0.0f, -1.0f, 0.0f, 1.0f, // Bottom-left
	    1.0f, -1.0f, 1.0f, 1.0f  // Bottom-right
	};

	static unsigned short sIndices[] =
	{
	    0, 1, 2,
	    2, 3, 1
	};

	Quad::WeakGLBufferPtr Quad::sIndexBuffer;
	Quad::WeakGLBufferPtr Quad::sVertexBuffer;

	Quad::Quad()
	{
    	PROCYON_DEBUG("Quad", "Allocating Quad." );

		// setup index bufer
		GLBufferPtr idxBuf;
		if ( !( idxBuf = sIndexBuffer.lock() ) )
		{
    		PROCYON_DEBUG("Quad", "Allocating shared index buffer." );
			sIndexBuffer = idxBuf
	        	= std::make_shared<GLBuffer>( sizeof(sIndices), sIndices );
		}
		SetIndexBuffer( idxBuf, sizeof( sIndices ) / sizeof( unsigned short ) );
		
		// setup vertex buffer
		GLBufferPtr vertBuf;
		if ( !( vertBuf = sVertexBuffer.lock() ) )
		{
    		PROCYON_DEBUG("Quad", "Allocating shared vertex buffer." );
			sVertexBuffer = vertBuf 
				= std::make_shared<GLBuffer>( sizeof(sVerts), sVerts );
		}
    	AddAttribute( "position", vertBuf, 2, 4*sizeof(float) );
    	AddAttribute( "texcoord", vertBuf, 2, 4*sizeof(float), 2*sizeof(float) );
	}

} /* namespace Procyon */
