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
#include "Renderer.h"
#include "RenderCore.h"
#include "FontFace.h"

namespace Procyon {

    bool sDebugText = false;

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

	void Text::SetText( const char* format, ... )
	{
		char dest[1024];
		va_list argptr;
		va_start( argptr, format );
		vsnprintf( dest, 1024, format, argptr );
		va_end( argptr );
		mText = dest;
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

	void Text::SetColor( const glm::vec4& color )
	{
		mColor = color;
	}

	const glm::vec4& Text::GetColor() const
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
        const FontMetrics metrics = mFont->GetMetrics( mFontSize );
		mDims = glm::vec2( 0.0f, metrics.line_height );

		glm::vec2 		pen;
		unsigned int 	prev = 0;

        if ( !mText.empty() )
        {
    		// Assumed ascii for now.
    		for( auto c : mText )
    		{
    			if ( c == '\n')
    			{
    				pen.x = 0.0f;
    				pen.y += metrics.line_height;
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
        else
        {
            mDims.x = 5.0f;
        }
	}

	const glm::vec2	Text::GetTextDimensions() const
	{
		return mDims;
	}

	void Text::PostRenderCommands( Renderer* r, RenderCore* rc ) const
	{
        const FontMetrics metrics = mFont->GetMetrics( mFontSize );

        if ( sDebugText )
        {
            const glm::vec2 baselineStart = mPosition + glm::vec2( 0.0f, -metrics.descender );
            if ( !mText.empty() )
                r->DrawLine( baselineStart, baselineStart + glm::vec2( mDims.x, 0.0f ), glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
            r->DrawWireframeRect( Rect( mPosition + glm::vec2(0.0f, 0.5f), glm::vec2( mDims.x, -mDims.y ) ), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ), true );
        }

		float x = 0.0f;
		float y = (float)-metrics.descender;

		unsigned int prev = 0;

		// Assumed ascii for now.
		for( auto c : mText )
		{
			if ( c == '\n')
			{
				x = 0.0f;
				y += mFont->GetMetrics( mFontSize ).line_height;
				prev = 0;
				continue;
			}
			else if ( c == '\t' )
			{
				// do something with tabs?
			}
			//TODO: Spaces are handled as quads... probably wrong.

            const Glyph* g = mFont->GetGlyph( mFontSize, c );

            if ( !g )
            {
            	continue; // unsupported character
            }

            if ( prev )
            {
            	x += mFont->GetKerning( mFontSize, prev, c );
            }

            BatchedQuad quaddata;
	        quaddata.position[0] = mPosition.x + x + g->center.x;
	        quaddata.position[1] = mPosition.y + y + g->center.y;
	        quaddata.size[0]     = g->size.x;
	        quaddata.size[1]     = g->size.y;
	        quaddata.rotation    = mOrientation;
	        quaddata.uvoffset[0] = (float)g->atlas_offset.s;
	        quaddata.uvoffset[1] = (float)g->atlas_offset.t;
	        quaddata.uvsize[0]   = (float)g->atlas_size.s;
	        quaddata.uvsize[1]   = (float)g->atlas_size.t;
	        quaddata.color[0] 	 = mColor.x;
	        quaddata.color[1] 	 = mColor.y;
	        quaddata.color[2] 	 = mColor.z;
			quaddata.color[3] 	 = mColor.w;
			quaddata.origin[0]	 = mOrigin.x;
			quaddata.origin[1]	 = mOrigin.y;

	        RenderCommand cmd;
	        cmd.op               = RENDER_OP_QUAD;
	        cmd.texture          = mFont->GetTexture( mFontSize );
	        cmd.instancecount    = 1;
	        cmd.quaddata         = &quaddata;
	        cmd.flags 		 	 = RENDER_SCREEN_SPACE;
	        rc->AddOrAppendCommand( cmd );

            x += g->advance;

            prev = c;
		}
	}

} /* namespace Procyon */
