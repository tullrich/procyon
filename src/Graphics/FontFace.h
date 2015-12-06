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
#include "stb_truetype.h"

#define GLYPH_COUNT 96

namespace Procyon {

	namespace GL { class GLTexture;	}

	struct Glyph
	{
		glm::vec2 	center;	// glyph center, baseline relative
		glm::vec2 	size;	// glyph size, baseline relative (?)
		glm::vec2 	uvoff;	// uv offset, bottom-left?
		glm::vec2 	uvsize;	// uv box size
		float 		advance;// x-axis advance
	};

	class CachedFontSize
	{
	public:
		unsigned int 		size;
		Glyph 				glyphs[ GLYPH_COUNT ];
    	GL::GLTexture*   	atlas;
    	float 				newlinegap;

		void 				InitGlyphs( const stbtt_fontinfo* info );
		void 				Rasterize( unsigned char* data );
	};

	class FontFace
	{
	public:
								FontFace( const std::string& filepath, unsigned int fontsize );
								~FontFace();

		const GL::GLTexture* 	GetTexture( unsigned int fontsize ) const;
		const Glyph* 			GetGlyph( unsigned int fontsize, unsigned int c ) const;
		float 					GetKerning( unsigned int fontsize, unsigned int cb1, unsigned int cb2 ) const;
		float 					GetNewLineGap( unsigned int fontsize ) const;
		void 					EnsureCached( unsigned int fontsize ) const;

		float 					GetMaxGlyphWidth( unsigned int fontsize ) const;

	protected:
		bool 					BufferFile( const std::string& filepath, std::vector<char>& buffer );


		typedef std::unordered_map<unsigned int, CachedFontSize*> FontSizeTable;
		mutable FontSizeTable 			mCache;

		std::vector<char>				mBuffer;
    	stbtt_fontinfo 					mFontInfo;
	};

	FontFace* 		CreateFontFace( const std::string& fontpath );
	const FontFace* GetOrCreateFontFace( std::string fontpath );

} /* Procyon */

#endif /* _FONT_FACE_H */
