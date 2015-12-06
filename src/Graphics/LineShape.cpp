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
#include "LineShape.h"
#include "GLProgram.h"
#include "GLBuffer.h"
#include "GLGeometry.h"

using namespace Procyon::GL;

namespace Procyon {

	static float sVerts[] = 
	{
	    //  Position 
	    0.0f, 0.0f, // start
	    1.0f, 0.0f, // end
	};

	static unsigned short sIndices[] =
	{
	    0, 1
	};

    static GLGeometry* CreateLineGeometry()
    {
        GLGeometry* geo = new GLGeometry();

        static GLBufferPtr idxBuf;
        if ( !idxBuf )
        {
            idxBuf
                = std::make_shared<GLBuffer>( sizeof(sIndices), sIndices );
        }
        geo->SetIndexBuffer( idxBuf
            , sizeof( sIndices ) / sizeof( unsigned short ), GL_LINES );

        GLBufferPtr vertBuf 
            = std::make_shared<GLBuffer>( sizeof(sVerts), sVerts );
        geo->AddAttribute( "position", vertBuf, 2, 2*sizeof(float));

        return geo;
    }

    LineShape::LineShape( const glm::vec2& start, const glm::vec2& end )
        : mStart( start )
        , mEnd( end )
        , mGeometry( CreateLineGeometry() )
        , Shape( mGeometry )
    {
        mBuffer = mGeometry->GetAttribute( "position" );
    }

    void LineShape::SetStart( const glm::vec2& start )
    {
        mStart = start;
    }

    void LineShape::SetEnd( const glm::vec2& end )
    {
        mEnd = end;
    }

    void LineShape::Render( const GLProgram* program ) const
    {
        glm::vec2 verts[2] = { mStart, mEnd };
        mBuffer->SetData( sizeof(verts), verts );

        mGeometry->Bind( program );
        mGeometry->Draw();

        /*GLint attribLoc = program->GetAttributeLocation( "position" );

        glEnableVertexAttribArray( attribLoc );

        glBegin( GL_LINES );
            glVertexAttrib2f( attribLoc, mStart.x, mStart.y );
            glVertexAttrib2f( attribLoc, mEnd.x, mEnd.y );
        glEnd();

        glDrawElements( GL_LINES, 2, GL_UNSIGNED_SHORT, sIndices );*/
    }
} /* namespace Procyon */
