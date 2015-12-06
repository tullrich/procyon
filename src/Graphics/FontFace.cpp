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
#include "FontFace.h"
#include "Image.h"
#include "GLTexture.h"

using namespace Procyon::GL;

#define DEFAULT_FONT_IMG_X 512
#define DEFAULT_FONT_IMG_Y 512

namespace Procyon {

	void CachedFontSize::InitGlyphs( const stbtt_fontinfo* info )
	{
		float scale = stbtt_ScaleForPixelHeight( info, size );
		PROCYON_DEBUG( "FontFace", "stbtt_ScaleForPixelHeight %i pixel height scale: %f", size, scale );
/*
		int fx0, fy0, fx1, fy1;
		stbtt_GetFontBoundingBox( &info, &fx0, &fy0, &fx1, &fy1 );
*/
		int ascent, descent, lineGap;
		stbtt_GetFontVMetrics( info, &ascent, &descent, &lineGap );
		PROCYON_DEBUG( "FontFace", "stbtt_GetFontVMetrics ascent: %i descent: %i linegap: %i"
			, ascent, descent, lineGap );
		newlinegap = scale * ( ascent - descent + lineGap );

/*
		PROCYON_DEBUG( "FontFace", "stbtt_GetFontVMetrics scaled ascent: %f descent: %f linegap: %f"
			, ascent * scale, descent * scale, lineGap * scale );*/

		for ( int i = 0; i < GLYPH_COUNT; i++ )
		{
			int x0, y0, x1, y1;
			stbtt_GetCodepointBitmapBox( info, i + 32, scale, scale, &x0, &y0, &x1, &y1 );
			
			Glyph& g = glyphs[ i ];
			g.size   = glm::vec2( x1 - x0, y1 - y0 );
			g.center = glm::vec2( x1 - g.size.x / 2.0f, -y0 - g.size.y / 2.0f );
			g.center.y += -descent * scale;

			//PROCYON_DEBUG( "FontFace", "'%c' c <%f, %f> s <%f, %f> x0 %i x1 %i y0 %i y1 %i"
			//	, i + 32, g.center.x, g.center.y, g.size.x, g.size.y, x0, x1, y0, y1  );
		}
	}

	void CachedFontSize::Rasterize( unsigned char* data )
	{
	    MemoryImage img( DEFAULT_FONT_IMG_X, DEFAULT_FONT_IMG_Y, 1 );

	    stbtt_pack_context ctx;
    	stbtt_packedchar packedChars[ GLYPH_COUNT ];
	    stbtt_PackBegin( &ctx, img.MutableData(), img.Height(), img.Width(), 0, 1, 0 );
	    stbtt_PackFontRange( &ctx, data, 0, size, 32, GLYPH_COUNT, packedChars );
	    stbtt_PackEnd( &ctx );

	    for ( int i = 0; i < GLYPH_COUNT; i++ )
		{
			Glyph& g = glyphs[ i ];
			stbtt_packedchar& packed = packedChars[ i ];

			g.uvoff = glm::vec2( packed.x0 / (float)DEFAULT_FONT_IMG_X, packed.y0 / (float)DEFAULT_FONT_IMG_Y );
			g.uvsize = glm::vec2( packed.x1 - packed.x0, packed.y1 - packed.y0 ) 
				/ glm::vec2( DEFAULT_FONT_IMG_X, DEFAULT_FONT_IMG_Y );
			g.advance = packed.xadvance;
		}

	    atlas = new GLTexture( GL_TEXTURE_2D, img );
	    atlas->SetMinMagFilter( GL_LINEAR, GL_LINEAR );
	}

	FontFace::FontFace( const std::string& filepath, unsigned int fontsize )
	{
		PROCYON_DEBUG( "FontFace", "Allocating new FontFace '%s'.", filepath.c_str() );

		if ( !BufferFile( filepath, mBuffer ) )
		{
	        throw std::runtime_error("Unable to open font file '" + filepath + "'!" );
		}

		stbtt_InitFont( &mFontInfo, (unsigned char*)mBuffer.data(), 0 );
		PROCYON_DEBUG( "FontFace", "stbtt_InitFont glyphs '%i'.", mFontInfo.numGlyphs );

		EnsureCached( fontsize );
	}

	FontFace::~FontFace()
	{
		for ( auto pair : mCache )
		{
			CachedFontSize* fs = pair.second;
			delete fs->atlas;
			delete fs;
		}
	}

	void FontFace::EnsureCached( unsigned int fontsize ) const
	{
		auto search = mCache.find( fontsize );

		if ( search == mCache.end() )
		{
			CachedFontSize* fs = new CachedFontSize();
			memset( (void*)fs, 0, sizeof( CachedFontSize ) );

			mCache[ fontsize ] 	= fs;
			fs->size 			= fontsize;

			fs->InitGlyphs( &mFontInfo );
			fs->Rasterize( (unsigned char*)mBuffer.data() );
		}
	}

	float FontFace::GetMaxGlyphWidth( unsigned int fontsize ) const
	{
		float scale = stbtt_ScaleForPixelHeight( &mFontInfo, fontsize );

		int x0, y0, x1, y1;
		stbtt_GetFontBoundingBox( &mFontInfo, &x0, &y0, &x1, &y1 );
		return scale * x1;
	}

	bool FontFace::BufferFile( const std::string& filepath, std::vector<char>& buffer )
	{
		std::fstream file( filepath, std::ios::binary | std::ios::in );
	    if ( !file )
	    {
	    	return false;
	    }

	    buffer.assign( ( std::istreambuf_iterator<char>( file ) )
	        , ( std::istreambuf_iterator<char>() ) );
	    PROCYON_DEBUG( "FontFace", "Buffering %i bytes.", buffer.size() );
	    return true;
	}

	const GL::GLTexture* FontFace::GetTexture( unsigned int fontsize ) const
	{
		auto search = mCache.find( fontsize );
		if ( search == mCache.end() )
			return NULL;
		return search->second->atlas;
	}

	const Glyph* FontFace::GetGlyph( unsigned int fontsize, unsigned int c ) const
	{
		if ( c < 32 || c >= (32 + GLYPH_COUNT) )
		{
			// unsupported character, return the '?' glyph
			// as the replacement character.
			c = 0x3F;
		}
		
		auto search = mCache.find( fontsize );
		if ( search == mCache.end() )
			return NULL; // Font size not found in the cache... for now just render nothing.

		return &search->second->glyphs[ c - 32 ];
	}

	float FontFace::GetKerning( unsigned int fontsize, unsigned int cb1, unsigned int cb2 ) const
	{
		float scale = stbtt_ScaleForPixelHeight( &mFontInfo, fontsize );
		return scale * stbtt_GetCodepointKernAdvance( &mFontInfo, cb1, cb2 );
	}

	float FontFace::GetNewLineGap( unsigned int fontsize ) const
	{
		auto search = mCache.find( fontsize );
		if ( search == mCache.end() )
			return 0.0f;

		return search->second->newlinegap;

	}

	FontFace* CreateFontFace( const std::string& fontpath )
	{
	    return new FontFace( fontpath, 2 );
	}

	typedef std::unordered_map< std::string, FontFace* > FontCache;
	static FontCache gFontCache;

const FontFace* GetOrCreateFontFace( std::string fontpath )
	{
		//std::transform(fontpath.begin(), fontpath.end(), fontpath.begin(), ::tolower);

		auto search = gFontCache.find( fontpath );
		if( search != gFontCache.end() )
		{
			PROCYON_DEBUG( "FontFace", "FontFace '%s' found in cache.", fontpath.c_str() );
			return search->second;
		}

		FontFace* ff = CreateFontFace( fontpath );
		gFontCache[ fontpath ] = ff;
		return ff;
	}

} /* Procyon */
