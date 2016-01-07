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
#include "Text.h"
#include "RenderCore.h"
#include "FontFace.h"

namespace Procyon {

	Text::Text( const std::string& text, const FontFace* font, unsigned int fontsize /*= 32*/ )
		: mFont( font )
		, mFontSize( fontsize )
	{
		mFont->EnsureCached( mFontSize );
		SetText( text );
	}

	Text::Text( const FontFace* font, unsigned int fontsize /*= 32*/ )
		: mFont( font )
		, mFontSize( fontsize )
	{
		mFont->EnsureCached( mFontSize );
	}

	Text::~Text()
	{
	}

	const std::string& Text::GetText() const
	{
		return mText;
	}

	void Text::SetText( const std::string& str )
	{
		mText = str;
		RecalculateDimensions();
	}

	void Text::Append( const std::string& str )
	{
		mText.append( str );
		RecalculateDimensions();
	}

	void Text::EraseBack( size_t count /* = 1 */ )
	{
		mText.erase( mText.end() - glm::min( mText.size(), (size_t)count ), mText.end() );
		RecalculateDimensions();
	}

	size_t Text::CharacterCount() const
	{
		// Assumed ascii for now
		return mText.size();
	}

	const FontFace* Text::GetFont() const
	{
		return mFont;
	}

	void Text::SetFont( const FontFace* font )
	{
		mFont = font;
		mFont->EnsureCached( mFontSize );
		RecalculateDimensions();
	}

	void Text::SetColor( const glm::vec3& color )
	{
		mColor = color;
	}

	const glm::vec3& Text::GetColor() const
	{
		return mColor;
	}

	void Text::SetFontSize( unsigned int fontsize )
	{
		mFontSize = fontsize;
		mFont->EnsureCached( fontsize );
		RecalculateDimensions();
	}

	unsigned int Text::GetFontSize() const
	{
		return mFontSize;
	}

	void Text::RecalculateDimensions()
	{
		mDims = glm::vec2( 0.0f );

		glm::vec2 		pen;
		unsigned int 	prev = 0;

		// Assumed ascii for now.
		for( auto c : mText )
		{
			if ( c == '\n')
			{
				pen.x = 0.0f;
				pen.y += mFont->GetNewLineGap( mFontSize );
				prev = 0;
				continue;
			}
			else if ( c == '\t' )
			{
				// do something with tabs?
			}

            const Glyph* g = mFont->GetGlyph( mFontSize, c );

            if ( !g )
            {
            	continue; // unsupported character
            }

            if ( prev )
            {
            	pen.x += mFont->GetKerning( mFontSize, prev, c );
            }

            pen.x += g->advance;
            prev = c;

            mDims = glm::max( mDims, pen );
		}
	}

	const glm::vec2	Text::GetTextDimensions() const
	{
		return mDims;
	}

	void Text::PostRenderCommands( RenderCore* rc ) const
	{
		float x = 0.0f;
		float y = 0.0f;

		unsigned int prev = 0;

		// Assumed ascii for now.
		for( auto c : mText )
		{
			if ( c == '\n')
			{
				x = 0.0f;
				y -= mFont->GetNewLineGap2( mFontSize );
				prev = 0;
				continue;
			}
			else if ( c == '\t' )
			{
				// do something with tabs?
			}
			//TODO: Spaces are handled as quads... probably wrong.

            const Glyph* g = mFont->GetGlyph2( mFontSize, c );

            if ( !g )
            {
            	continue; // unsupported character
            }

            if ( prev )
            {
            	x += mFont->GetKerning2( mFontSize, prev, c );
            }

            BatchedQuad quaddata;
	        quaddata.position[0] = mPosition.x + x + g->center.x;
	        quaddata.position[1] = mPosition.y + y + g->center.y;
	        quaddata.size[0]     = g->size.x;
	        quaddata.size[1]     = g->size.y;
	        quaddata.rotation    = mOrientation;
	        quaddata.uvoffset[0] = g->uvoff.s;
	        quaddata.uvoffset[1] = g->uvoff.t;
	        quaddata.uvsize[0]   = g->uvsize.s;
	        quaddata.uvsize[1]   = g->uvsize.t;
	        quaddata.tint[0] 	 = mColor.x;
	        quaddata.tint[1] 	 = mColor.y;
	        quaddata.tint[2] 	 = mColor.z;

	        RenderCommand cmd;
	        cmd.op               = RENDER_OP_QUAD;
	        cmd.program          = NULL;
	        cmd.texture          = mFont->GetTexture2( mFontSize );
	        cmd.instancecount    = 1;
	        cmd.quaddata         = &quaddata;
	        cmd.flags 		 	 = RENDER_SCREEN_SPACE;
	        rc->AddOrAppendCommand( cmd );

            x += g->advance;

            prev = c;
		}

	    glm::vec2 quaddata[] =
	    {
	        glm::vec2( mPosition.x, mPosition.y + 32.0f ), // v0
	        glm::vec2( mPosition.x + 32.0f, mPosition.y + 32.0f ), // v1
	        glm::vec2( mPosition.x, mPosition.y ), // v2
	        glm::vec2( mPosition.x + 32.0f, mPosition.y ), // v3
	    };

	    // v0, v1, v2
	    // v2, v1, v3

	    RenderCommand cmd;
	    cmd.op               = RENDER_OP_PRIMITIVE;
	    cmd.program          = NULL;
	    cmd.primmode         = PRIMITIVE_QUAD;
	    cmd.verts            = (PrimitiveVertex*) &quaddata[0];
	    cmd.vertcount        = 4;
	    cmd.flags            = RENDER_SCREEN_SPACE;
	    cmd.color[0]         = mColor.x;
	    cmd.color[1]         = mColor.y;
	    cmd.color[2]         = mColor.z;
	    cmd.color[3]         = 1.0f;
	    //rc->AddOrAppendCommand( cmd );
	}

} /* namespace Procyon */
