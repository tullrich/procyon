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
#include "Texture.h"

#include "stb_rect_pack.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define DEFAULT_FONT_IMG_X 256
#define DEFAULT_FONT_IMG_Y 256

#define FONT_IMG_DIM_MAX 2048

static stbrp_node sWorkspace[ FONT_IMG_DIM_MAX ];
static bool sFreeTypeInitialized;
static FT_Library sFreeType;

static void DumpFace( FT_Face face, int size )
{
    /* face: global metrics, unhinted & manually scaled */
    const int units_per_EM = face->units_per_EM;
    const int newlinegap = ( int )( glm::round((size * face->height) / (float)units_per_EM ) );
    const int ascender = ( int )( glm::round((size * face->ascender) / (float)units_per_EM ) );
    const int descender = ( int )( glm::round((size * face->descender) / (float)units_per_EM ) );
    PROCYON_DEBUG( "FontFace", "face: height %i ascender %i descender %i"
        , newlinegap
        , ascender
        , descender );

    /* face->size->metrics: sized metrics, potentially hinted */
    PROCYON_DEBUG( "FontFace", "face->size->metrics: height %i.%i ascender %i.%i descender %i.%i "
        , face->size->metrics.height >> 6
        , face->size->metrics.height & 0x3F
        , face->size->metrics.ascender >> 6
        , face->size->metrics.ascender & 0x3F
        , face->size->metrics.descender >> 6
        , face->size->metrics.descender & 0x3F );

    /* face->bbox: even more values */
    const glm::ivec2 min = ( glm::ivec2( face->bbox.xMin * size, face->bbox.yMin * size ) ) / units_per_EM;
    const glm::ivec2 max = ( glm::ivec2( face->bbox.xMax * size, face->bbox.yMax * size  ) ) / units_per_EM;
    const int glyph_height = max.y - min.y;
    PROCYON_DEBUG( "FontFace", "face->bbox: height %i [xmin %i xmax %i] [ymin %i ymax %i]\n"
        , glyph_height, min.x, max.x, min.y, max.y );
}

static void DumpGlyph( FT_Bitmap& bitmap )
{
    assert( bitmap.pixel_mode == FT_PIXEL_MODE_GRAY );

    PROCYON_DEBUG( "FontFace", "Rows %i Width %i Pitch %i Pixel Mode %i "
        , bitmap.rows, bitmap.width, bitmap.pitch, bitmap.pixel_mode );

    unsigned char *buf = (unsigned char *)malloc( bitmap.width + 1 );
    for ( unsigned r = 0; r < bitmap.rows; ++r )
    {
        unsigned c = 0;
        for ( c = 0; c <  bitmap.width; ++c  )
        {
            if ( bitmap.buffer[ r * bitmap.pitch + c ] > 0 )
            {
                buf[ c ] = 'A';
            }
            else
            {
                buf[ c ] = ' ';
            }
        }
        buf[ c ] = 0;
        PROCYON_DEBUG( "FontFace", "%s", buf );
    }
    free( buf );
}

namespace Procyon {

	CachedFontSize::CachedFontSize( unsigned int fontsize, FT_Face face )
		: size( fontsize )
		, atlas( NULL )
	{
		memset( (void*)glyphs, 0, sizeof( atlas ) );

        PROCYON_DEBUG( "FontFace", "CachedFontSize size %i", fontsize );

        /* input freetype pixel size */
		FT_Error error = 0;
		if ( ( error = FT_Set_Pixel_Sizes( face, 0, fontsize ) ) != FT_Err_Ok )
		{
			PROCYON_ERROR( "FontFace", "FT_Set_Pixel_Sizes error with size %i", fontsize );
            return;
		}

        DumpFace( face, fontsize );

        /* freetype first pass, compute the hinted glyph metrics */
        if ( !CacheGlyphMetrics( face ) )
            return;

        /* compute glyph locations in the atlas- retry with larger dimensions upon failed pack */
        glm::ivec2 dims = glm::ivec2( DEFAULT_FONT_IMG_X, DEFAULT_FONT_IMG_Y );
        while ( !ComputeAtlasPacking( dims ) )
        {
            PROCYON_WARN( "FontFace", "Atlas packing failed at atlas size %ix%i", dims.x, dims.y );
            dims *= 2;
            if ( dims.x > FONT_IMG_DIM_MAX || dims.y > FONT_IMG_DIM_MAX )
            {
    			PROCYON_ERROR( "FontFace", "Unable to pack font atlas for font size %i", fontsize );
                return;
            }
        };

        /*  create atlas and upload to gpu */
        if ( !Rasterize( face, dims) )
            return;
	}

    bool CachedFontSize::CacheGlyphMetrics( FT_Face face )
    {
        metrics.max_advance = face->size->metrics.max_advance >> 6;
        metrics.ascender = face->size->metrics.ascender >> 6;
        metrics.descender = face->size->metrics.descender >> 6;
        metrics.line_height = metrics.ascender - metrics.descender;  // force that line_height = ascender + descender.
        // Ignore the face->size->metrics.height =(

        /* foreach glyph */
        for ( unsigned char code = 32; code < GLYPH_COUNT + 32; code++ )
        {
            if ( FT_Load_Char( face, code, FT_LOAD_RENDER ) != FT_Err_Ok )
            {
    			PROCYON_ERROR( "FontFace", "FT_Load_Char error on char '%c'", code );
                return false; // failure
            }

            //int baseline_offset = int((float)min.y + (float)glyph_height / 2.0f);

            Glyph& glyph = glyphs[ code - 32 ];
            glyph.size = glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows );
            glyph.center = (glyph.size / 2.0f - glm::vec2(-face->glyph->bitmap_left, glyph.size.y - face->glyph->bitmap_top));
            glyph.advance = ( float )( face->glyph->advance.x >> 6 );

            //PROCYON_DEBUG( "FontFace", "Codepoint '%c' advance %f width %f height %f left %f top %f"
            //    , code, glyph.advance, glyph.size.x, glyph.size.y, glyph.center.x, glyph.center.y );
        }

        return true; // success
    }

    bool CachedFontSize::ComputeAtlasPacking( glm::ivec2 dims )
    {
        assert( dims.x <= FONT_IMG_DIM_MAX && dims.y <= FONT_IMG_DIM_MAX );

		stbrp_context context;
		stbrp_init_target( &context, dims.x, dims.y, sWorkspace, FONT_IMG_DIM_MAX );

		stbrp_rect rects[ GLYPH_COUNT ];
        int packc = 0;
		for ( int i = 0; i < GLYPH_COUNT; ++i )
		{
            Glyph& glyph = glyphs[ i ];
            if ( glyph.size.x > 0 && glyph.size.y > 0 )
            {
			    rects[ packc ].id = i;
    			rects[ packc ].w = (int)glyph.size.x + 2;
    			rects[ packc ].h = (int)glyph.size.y + 2;
    			rects[ packc ].x = 0;
    			rects[ packc ].y = 0;
    			rects[ packc ].was_packed = 0;
                packc++;
            }
		}

		stbrp_pack_rects( &context, rects, packc );

		for ( int i = 0; i < packc; ++i )
		{
			stbrp_rect& r = rects[ i ];
			if ( !r.was_packed )
			{
                PROCYON_WARN( "FontFace", "Failed to pack char '%c'", r.id + 32 );
                return false;
			}

            Glyph& glyph = glyphs[ r.id ];
            glyph.pixel_offset = glm::vec2( r.x + 1, r.y + 1 );
            glyph.uvoff = glm::vec2( glyph.pixel_offset ) / glm::vec2( dims );
            glyph.uvsize = glm::vec2( r.w - 2, r.h - 2 ) / glm::vec2( dims );
		}

        return true;
    }

    bool CachedFontSize::Rasterize( FT_Face face, glm::ivec2 dims )
    {
        MutableImage img( dims.x, dims.y, 1 );
        unsigned char* data = img.MutableData();

        for ( unsigned char code = 0; code < GLYPH_COUNT; code++ )
        {
			Glyph& g = glyphs[ code ];

            if ( (int)g.size.x == 0 || (int)g.size.y == 0 )
                continue; // no pixels in atlas

            if ( FT_Load_Char( face, code + 32, FT_LOAD_RENDER ) != FT_Err_Ok )
            {
    			PROCYON_ERROR( "FontFace", "FT_Load_Char error on char '%c'", code + 32 );
                return false;
            }
            FT_Bitmap& bitmap = face->glyph->bitmap;
            assert( bitmap.pixel_mode == FT_PIXEL_MODE_GRAY );
            assert( bitmap.pitch == g.size.x );

            for ( int w = 0; w < g.size.x; ++w )
            {
                for ( int h = 0; h < g.size.y; ++h )
                {
                    data[ (h + (int)g.pixel_offset.y) * dims.x + w + (int)g.pixel_offset.x ] = bitmap.buffer[ h * bitmap.pitch + w ];
                }
            }
            //PROCYON_DEBUG( "FontFace", "Rasterized glyph '%c' uvoff <%f, %f> uvsize <%f, %f>"
            //    , code + 32, g.uvoff.x, g.uvoff.y, g.uvsize.x, g.uvsize.y );
        }

	    atlas = Texture::Allocate( img );
	    atlas->SetMinMagFilter( FILTER_LINEAR, FILTER_LINEAR);
        return true; // success
    }

	FontFace::FontFace( const std::string& filepath, unsigned int fontsize )
		: mFace( NULL )
	{
		PROCYON_DEBUG( "FontFace", "Allocating new FontFace '%s'.", filepath.c_str() );

		FT_Error error = 0;
		if ( !sFreeTypeInitialized )
		{
			if ( (error = FT_Init_FreeType( &sFreeType ) ) != FT_Err_Ok )
			{
				PROCYON_ERROR( "FontFace", "FT_Init_FreeType error" );
                return;
			}
		}

		if( ( error = FT_New_Face( sFreeType, filepath.c_str(), 0, &mFace ) ) != FT_Err_Ok )
		{
			PROCYON_ERROR( "FontFace", "FT_New_Face error" );
            return;
		}

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

		FT_Done_Face( mFace );
	}

	void FontFace::EnsureCached( unsigned int fontsize ) const
	{
		auto search = mCache.find( fontsize );

		if ( search == mCache.end() )
		{
			mCache[ fontsize ] = new CachedFontSize( fontsize, mFace );
		}
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

    const Texture* FontFace::GetTexture( unsigned int fontsize ) const
    {
        EnsureCached( fontsize );
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

	int FontFace::GetKerning( unsigned int fontsize, unsigned int cb1, unsigned int cb2 ) const
	{
        if ( !mFace || !FT_HAS_KERNING( mFace ) )
            return 0;

        FT_Set_Pixel_Sizes( mFace, 0, fontsize );

        FT_UInt idx1 = FT_Get_Char_Index( mFace, cb1 );
        FT_UInt idx2 = FT_Get_Char_Index( mFace, cb2 );

        FT_Vector kern;
        if ( FT_Get_Kerning( mFace, idx1, idx2, FT_KERNING_DEFAULT, &kern  ) != FT_Err_Ok )
        {
            PROCYON_DEBUG( "FontFace", "Kern Error" );
            return 0;
        }

		return kern.x >> 6;
	}

	FontMetrics FontFace::GetMetrics( unsigned int fontsize ) const
	{
		auto search = mCache.find( fontsize );
		if ( search == mCache.end() )
			return FontMetrics();

		return search->second->metrics;
	}

	FontFace* CreateFontFace( const std::string& fontpath )
	{
	    return new FontFace( fontpath, 20 );
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
