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
#ifndef _FONT_FACE_H
#define _FONT_FACE_H

#include "ProcyonCommon.h"

#define GLYPH_COUNT 96

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;

namespace Procyon {

	class Texture;

	struct Glyph
	{
		glm::vec2 	center;	// glyph center, baseline relative
		glm::vec2 	size;	// glyph size, baseline relative (?)
        glm::ivec2  pixel_offset;

		glm::vec2 	uvoff;	// uv offset, bottom-left?
		glm::vec2 	uvsize;	// uv box size

		float 		    advance;// x-axis advance
	};

    struct FontMetrics
    {
        FontMetrics()
            : max_advance( 0 )
            , line_height( 0 )
            , ascender( 0 )
            , descender( 0 )
        {
        }

        int     max_advance;
        int     line_height;
        int     ascender;
        int     descender;
    };

	class CachedFontSize
	{
	public:
		unsigned int 		   size;
		Glyph 				       glyphs[ GLYPH_COUNT ];
    	Texture*   	 atlas;
        FontMetrics             metrics;

		CachedFontSize( unsigned int fontsize, FT_Face face );

    protected:
        bool  CacheGlyphMetrics( FT_Face face );
        bool ComputeAtlasPacking( glm::ivec2 dims );
        bool Rasterize( FT_Face face, glm::ivec2 dims );
	};

	class FontFace
	{
	public:
								FontFace( const std::string& filepath, unsigned int fontsize );
								~FontFace();

        const Texture*			GetTexture( unsigned int fontsize ) const;
		const Glyph*			GetGlyph( unsigned int fontsize, unsigned int c ) const;
		int						GetKerning( unsigned int fontsize, unsigned int cb1, unsigned int cb2 ) const;
		FontMetrics				GetMetrics( unsigned int fontsize ) const;
		void 					EnsureCached( unsigned int fontsize ) const;

	protected:
		bool 					BufferFile( const std::string& filepath, std::vector<char>& buffer );

		typedef std::unordered_map<unsigned int, CachedFontSize*> FontSizeTable;
		mutable FontSizeTable 			mCache;


    	FT_Face 						mFace;
	};

	FontFace* 		CreateFontFace( const std::string& fontpath );
	const FontFace* GetOrCreateFontFace( std::string fontpath );

} /* Procyon */

#endif /* _FONT_FACE_H */
