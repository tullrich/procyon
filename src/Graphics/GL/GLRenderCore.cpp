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
#include "GLRenderCore.h"
#include "GLTexture.h"
#include "GLProgram.h"
#include "GLBuffer.h"
#include "Graphics/Camera.h"

using namespace Procyon::GL;

static float data[] =
{
	-0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f
};

static unsigned short indices[] =
{
    0, 1, 2,
    2, 3, 0
};

// 30 MB
#define MAX_VERTEX_ATTRIB_BYTES 1024 * 1024 * 30
#define BATCH_STRIDE sizeof( BatchedQuad )

namespace Procyon {
namespace GL {

	static unsigned char 	gVertexAttribData[ MAX_VERTEX_ATTRIB_BYTES ];
	static int				gVertexDataWriteOffset 		= 0;

	bool operator==( const RenderCommand& rc1, const RenderCommand& rc2 )
	{
		if ( rc1.op == rc2.op && rc1.flags == rc2.flags)
		{
			switch ( rc1.op )
			{
			case RENDER_OP_QUAD: return rc1.texture == rc2.texture;
			case RENDER_OP_PRIMITIVE: return rc1.primmode == rc2.primmode && memcmp( rc1.color, rc2.color, 16 ) == 0;
			case RENDER_OP_POLYGON:
			case RENDER_OP_AA_LINE:
			default: false;
			}
		}
		return false;
	}

	static GLenum TranslatePrimitiveMode( PrimitiveMode pm )
	{
		switch ( pm )
		{
			case PRIMITIVE_LINE: 			return GL_LINES;
			case PRIMITIVE_TRIANGLE: 		return GL_TRIANGLES;
		}
		return GL_INVALID_ENUM;
	}

	GLRenderCore::GLRenderCore()
		: mRenderCommandCount( 0 )
	{
		mQuadBuffer 	= new GLBuffer( sizeof( data ), data );
		mQuadIndices 	= new GLBuffer( sizeof( indices ), indices );

		mBuffer 		= new GLBuffer( sizeof(gVertexAttribData), gVertexAttribData, GL_STREAM_DRAW );
		mOffBuffer 		= new GLBuffer( sizeof(gVertexAttribData), gVertexAttribData, GL_STREAM_DRAW );

   		mDefaultProg    = new GLProgram( "shaders/quadbatch.vert", "shaders/quadbatch.frag", { "UV0_ENABLED", "TEXTURE0_ENABLED" } );
		mTexturelessProg    = new GLProgram( "shaders/quadbatch.vert", "shaders/quadbatch.frag" );
   		mDefaultPrimitiveProg = new GLProgram( "shaders/primitive.vert", "shaders/primitive.frag" );
   		mDefaultPolygonProg = new GLProgram( "shaders/polygon.vert", "shaders/polygon.frag" );
		mDefaultLineProg = new GLProgram( "shaders/line.vert", "shaders/line.frag" );

		ResetStats();
	}

	GLRenderCore::~GLRenderCore()
	{
		delete mDefaultPolygonProg;
		delete mDefaultLineProg;
		delete mDefaultPrimitiveProg;
		delete mDefaultProg;
		delete mTexturelessProg;
		delete mOffBuffer;
		delete mBuffer;
		delete mQuadIndices;
		delete mQuadBuffer;
	}

	bool GLRenderCore::PushData( const unsigned char* data, int size )
	{
		if ( gVertexDataWriteOffset + size >= MAX_VERTEX_ATTRIB_BYTES )
		{
			PROCYON_WARN( "GLRenderCore", "MAX_VERTEX_ATTRIB_BYTES(%i) overflow, some quads will not be drawn!"
				, MAX_VERTEX_ATTRIB_BYTES);
			return false;
		}

		memcpy( gVertexAttribData + gVertexDataWriteOffset, data, size );
		gVertexDataWriteOffset += size;

		return true;
	}

	bool GLRenderCore::PushCommandData( const RenderCommand& rc )
	{
		switch ( rc.op )
		{
			case RENDER_OP_QUAD:
				return PushData( (const unsigned char*)rc.quaddata, rc.instancecount * sizeof( BatchedQuad ) );
			case RENDER_OP_PRIMITIVE:
				return PushData( (const unsigned char*)rc.verts, rc.vertcount * sizeof( PrimitiveVertex ) );
			case RENDER_OP_POLYGON:
				return PushData( (const unsigned char*)rc.colorverts, rc.colorvertcount * sizeof( ColorVertex ) );
			case RENDER_OP_AA_LINE:
				return PushData( (const unsigned char*)rc.lineverts, rc.linevertcount * sizeof( AALineVertex ) );
		}

		return false;
	}

	void GLRenderCore::AddCommand( const RenderCommand& cmd )
	{
		if ( mRenderCommandCount >= MAX_RENDER_CMDS - 1 )
		{
			PROCYON_WARN( "GLRenderCore", "MAX_RENDER_CMDS(%i) overflow, some quads will not be drawn!"
				, MAX_RENDER_CMDS);
			return;
		}

		RenderCommand& rc = mCmdBuffer[ mRenderCommandCount ];
		rc = cmd;
		rc.offset = gVertexDataWriteOffset;

		if ( PushCommandData( cmd ) )
		{
			mRenderCommandCount++;
		}
	}

	void GLRenderCore::AddOrAppendCommand( const RenderCommand& cmd )
	{
		if ( cmd.op == RENDER_OP_QUAD && mRenderCommandCount > 0 )
		{
			RenderCommand& prev = mCmdBuffer[ mRenderCommandCount - 1 ];
			if ( prev == cmd )
			{
				// append
				if ( PushCommandData( cmd ) )
				{
					prev.instancecount += cmd.instancecount;
				}
				return;
			}
		}
		else if ( cmd.op == RENDER_OP_PRIMITIVE && mRenderCommandCount > 0 )
		{

			RenderCommand& prev = mCmdBuffer[ mRenderCommandCount - 1 ];
			if ( prev == cmd )
			{
				// append
				if ( PushCommandData( cmd ) )
				{
					prev.vertcount += cmd.vertcount;
				}
				return;
			}
		}

		// fallback to just adding
		AddCommand( cmd );
	}

	void GLRenderCore::RenderQuadBatch( const RenderCommand& rc, const Camera2D& camera  )
	{
		const GLProgram* program = ( rc.texture ) ? mDefaultProg : mTexturelessProg;
		program->Bind();

		GLint sstLoc = program->GetUniformLocation( "screenSpaceTransform" );
		if ( rc.flags & RENDER_SCREEN_SPACE )
		{
			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( sstLoc, 1, false, glm::value_ptr( p ) );
		}
		else
		{
			glm::mat3 vp = camera.GetViewProjection();
			glUniformMatrix3fv( sstLoc, 1, false, glm::value_ptr( vp ) );
		}

		// Bind the screen dimensions
	    GLint dimsLoc = program->GetUniformLocation( "screenDims" );
		glm::vec2 screenDims = glm::vec2( camera.GetWidth(), camera.GetHeight() );
		glUniform2f( dimsLoc, screenDims.x, screenDims.y );

		// Bind the texture
		if ( rc.texture )
		{
			GLint texLoc = program->GetUniformLocation( "tex" );
			glUniform1i( texLoc, 0 );
			//glActiveTexture( GL_TEXTURE0 );
			rc.texture->Bind();
		}

		// Bind the immuatable quad position data
		mQuadIndices->Bind( GL_ELEMENT_ARRAY_BUFFER );
		mQuadBuffer->Bind( GL_ARRAY_BUFFER );
	    GLint vertPosLoc = program->GetAttributeLocation( "vertPosition" );
    	glVertexAttribPointer( vertPosLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0 );
	    glEnableVertexAttribArray( vertPosLoc );

		if ( rc.texture )
		{
			GLint uvLoc = program->GetAttributeLocation( "uv" );
    		glVertexAttribPointer( uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2)  );
			glEnableVertexAttribArray( uvLoc );
		}

		mBuffer->Bind( GL_ARRAY_BUFFER );

	    // Bind the quadbatch positions
	    GLint quadPosLoc = program->GetAttributeLocation( "quadPos" );
    	glVertexAttribPointer( quadPosLoc, 2, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(long)rc.offset );
    	glVertexAttribDivisor( quadPosLoc, 1 );
	    glEnableVertexAttribArray( quadPosLoc );

	    // Bind the quadbatch sizes
	    GLint quadSizeLoc = program->GetAttributeLocation( "quadSize" );
    	glVertexAttribPointer( quadSizeLoc, 2, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(sizeof(float) * 2 + rc.offset) );
    	glVertexAttribDivisor( quadSizeLoc, 1 );
	    glEnableVertexAttribArray( quadSizeLoc );

	    // Bind the quadbatch sizes
	    GLint quadRotLoc = program->GetAttributeLocation( "quadRotRads" );
    	glVertexAttribPointer( quadRotLoc, 1, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(sizeof(float) * 4 + rc.offset) );
    	glVertexAttribDivisor( quadRotLoc, 1 );
	    glEnableVertexAttribArray( quadRotLoc );

		GLint quadUVOffsetLoc = program->GetAttributeLocation( "quadUVOffset" );
		GLint quadUVSizeLoc = program->GetAttributeLocation( "quadUVSize" );
		if ( rc.texture )
		{
			// Bind the quadbatch uv position
			glVertexAttribPointer( quadUVOffsetLoc, 2, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(sizeof(float) * 5 + rc.offset) );
			glVertexAttribDivisor( quadUVOffsetLoc, 1 );
			glEnableVertexAttribArray( quadUVOffsetLoc );

			// Bind the quadbatch uv sizes
			glVertexAttribPointer( quadUVSizeLoc, 2, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(sizeof(float) * 7 + rc.offset) );
			glVertexAttribDivisor( quadUVSizeLoc, 1 );
			glEnableVertexAttribArray( quadUVSizeLoc );
		}

	    // Bind the quadbatch tint
	    GLint quadTintLoc = program->GetAttributeLocation( "quadTint" );
    	glVertexAttribPointer( quadTintLoc, 4, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(sizeof(float) * 9 + rc.offset) );
    	glVertexAttribDivisor( quadTintLoc, 1 );
	    glEnableVertexAttribArray( quadTintLoc );

	    // Bind the quadbatch positions
	    GLint quadOriginLoc = program->GetAttributeLocation( "quadOrigin" );
    	glVertexAttribPointer( quadOriginLoc, 2, GL_FLOAT, GL_FALSE, BATCH_STRIDE, (const void*)(sizeof(float) * 13 + rc.offset) );
    	glVertexAttribDivisor( quadOriginLoc, 1 );
	    glEnableVertexAttribArray( quadOriginLoc );

		glDrawElementsInstanced( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, rc.instancecount );

        glVertexAttribDivisor( quadPosLoc, 0 );
        glVertexAttribDivisor( quadSizeLoc, 0 );
        glVertexAttribDivisor( quadRotLoc, 0 );
        glVertexAttribDivisor( quadUVOffsetLoc, 0 );
        glVertexAttribDivisor( quadUVSizeLoc, 0 );
        glVertexAttribDivisor( quadTintLoc, 0 );
		glVertexAttribDivisor( quadOriginLoc, 0 );

		mFrameStats.batches++;
		mFrameStats.totalquads += rc.instancecount;
		if ( mFrameStats.batchmin != 0 )
		{
			mFrameStats.batchmin = glm::min( mFrameStats.batchmin, rc.instancecount );
		}
		else
		{
			mFrameStats.batchmin = rc.instancecount;
		}
		mFrameStats.batchmax = glm::max( mFrameStats.batchmax, rc.instancecount );
	}

	void GLRenderCore::RenderPrimitive( const RenderCommand& rc, const Camera2D& camera  )
	{
		const GLProgram* program = mDefaultPrimitiveProg;
		program->Bind();

		GLint mvMatLoc = program->GetUniformLocation( "modelViewMat" );
		GLint projLoc = program->GetUniformLocation( "projectionMat" );
		if ( rc.flags & RENDER_SCREEN_SPACE )
		{
			glm::mat3 identity;
			glUniformMatrix3fv( mvMatLoc, 1, false, glm::value_ptr( identity ) );

			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( projLoc, 1, false, glm::value_ptr( p ) );
		}
		else
		{
			glm::mat3 mv = camera.GetView();
			glUniformMatrix3fv( mvMatLoc, 1, false, glm::value_ptr( mv ) );

			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( projLoc, 1, false, glm::value_ptr( p ) );
		}

		GLint colorLoc = program->GetUniformLocation( "color" );
		glUniform4fv( colorLoc, 1, rc.color );

		mBuffer->Bind( GL_ARRAY_BUFFER );
	    GLint vertPosLoc = program->GetAttributeLocation( "vertPosition" );
    	glVertexAttribPointer( vertPosLoc, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(long)rc.offset );
	    glEnableVertexAttribArray( vertPosLoc );
    	glDrawArrays( TranslatePrimitiveMode( rc.primmode ), 0, rc.vertcount );

    	mFrameStats.batches++;
		mFrameStats.totalprimitives++;
	}

	void GLRenderCore::RenderPolygon( const RenderCommand& rc, const Camera2D& camera  )
	{
		const GLProgram* program = mDefaultPolygonProg;
		program->Bind();

		GLint mvMatLoc = program->GetUniformLocation( "u_mv_matrix" );
		GLint projLoc = program->GetUniformLocation( "u_p_matrix" );
		if ( rc.flags & RENDER_SCREEN_SPACE )
		{
			glm::mat3 identity;
			glUniformMatrix3fv( mvMatLoc, 1, false, glm::value_ptr( identity ) );

			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( projLoc, 1, false, glm::value_ptr( p ) );
		}
		else
		{
			glm::mat3 mv = camera.GetView();
			glUniformMatrix3fv( mvMatLoc, 1, false, glm::value_ptr( mv ) );

			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( projLoc, 1, false, glm::value_ptr( p ) );
		}

		mBuffer->Bind( GL_ARRAY_BUFFER );
	    GLint vertPosLoc = program->GetAttributeLocation( "vertPosition" );
    	glVertexAttribPointer( vertPosLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(long)rc.offset );
	    glEnableVertexAttribArray( vertPosLoc );
	    GLint colorLoc = program->GetAttributeLocation( "vertColor" );
    	glVertexAttribPointer( colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(sizeof(float) * 2 + rc.offset) );
	    glEnableVertexAttribArray( colorLoc );

    	glDrawArrays( GL_TRIANGLES, 0, rc.colorvertcount );

		mFrameStats.batches++;
	}

	void GLRenderCore::RenderAntiAliasedLine( const RenderCommand& rc, const Camera2D& camera  )
	{
		const GLProgram* program = mDefaultLineProg;
		program->Bind();

		GLint widthLoc = program->GetUniformLocation( "u_linewidth" );
		GLint mvLoc = program->GetUniformLocation( "u_mv_matrix" );
		GLint projLoc = program->GetUniformLocation( "u_p_matrix" );
		GLint colorLoc = program->GetUniformLocation( "color" );
		GLint featherLoc = program->GetUniformLocation( "feather" );

		if ( rc.flags & RENDER_SCREEN_SPACE )
		{
			glm::mat3 identity;
			glUniformMatrix3fv( mvLoc, 1, false, glm::value_ptr( identity ) );

			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( projLoc, 1, false, glm::value_ptr( p ) );
		}
		else
		{
			glm::mat3 mv = camera.GetView();
			glUniformMatrix3fv( mvLoc, 1, false, glm::value_ptr( mv ) );

			glm::mat3 p = camera.GetProjection();
			glUniformMatrix3fv( projLoc, 1, false, glm::value_ptr( p ) );
		}
		glUniform1fv( widthLoc, 1, &rc.linewidth );
		glUniform1fv( featherLoc, 1, &rc.feather );
		glUniform4fv( colorLoc, 1, rc.linecolor );

		mBuffer->Bind( GL_ARRAY_BUFFER );
	    GLint vertPosLoc = program->GetAttributeLocation( "vertPosition" );
    	glVertexAttribPointer( vertPosLoc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(long)rc.offset );
	    glEnableVertexAttribArray( vertPosLoc );
	    GLint normloc = program->GetAttributeLocation( "vertNormal" );
    	glVertexAttribPointer( normloc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2 + rc.offset) );
	    glEnableVertexAttribArray( normloc );

    	glDrawArrays( GL_TRIANGLE_STRIP, 0, rc.vertcount );

		mFrameStats.batches++;
	}

	bool GLRenderCore::RenderCommandsPending() const
	{
		return mRenderCommandCount != 0;
	}

	void GLRenderCore::Flush( const Camera2D& camera )
	{
		// early out if nothng is queued.
		if ( !RenderCommandsPending() )
			return;

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

		// Upload the data
		mBuffer->SetData( gVertexDataWriteOffset, gVertexAttribData, GL_STREAM_DRAW );

		for ( int i = 0; i < mRenderCommandCount; i++ )
		{
			RenderCommand& rc = mCmdBuffer[ i ];

			switch( rc.op )
			{
				case RENDER_OP_QUAD:
				{
					RenderQuadBatch( rc, camera );
					break;
				}
				case RENDER_OP_PRIMITIVE:
				{
					RenderPrimitive( rc, camera );
					break;
				}
				case RENDER_OP_POLYGON:
				{
					RenderPolygon( rc, camera );
					break;
				}
				case RENDER_OP_AA_LINE:
				{
					RenderAntiAliasedLine( rc, camera );
					break;
				}
			}
		}

		mRenderCommandCount = 0;
		gVertexDataWriteOffset = 0;

		std::swap( mBuffer, mOffBuffer );

		glDisable(GL_DEPTH_TEST);
	}

	void GLRenderCore::ResetStats()
	{
		mFrameStats.batches 		= 0;
		mFrameStats.batchmax 		= 0;
		mFrameStats.batchmin 		= 0;
		mFrameStats.totalquads 		= 0;
		mFrameStats.totalprimitives = 0;
	}

	const RenderFrameStats& GLRenderCore::GetFrameStats() const
	{
		return mFrameStats;
	}

} /* namespace GL


	/*static*/ RenderCore* RenderCore::Allocate()
	{
		return new GL::GLRenderCore();
	}

} /* namespace Procyon */
