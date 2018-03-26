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
#ifndef _TEXT_H
#define _TEXT_H

#include "ProcyonCommon.h"
#include "Transformable.h"
#include "Renderable.h"

namespace Procyon {

	class FontFace;

	class Text : public Transformable, public Renderable
	{
	public:
							Text( const std::string& text, const FontFace* font, unsigned int fontsize = 32 );
							Text( const FontFace* font, unsigned int fontsize = 32 );
		virtual 			~Text();

		const std::string& 	GetText() const;
		void 				SetText( const std::string& str );
		void 				SetText( const char* format, ... );
		void 				Append( const std::string& str );
		void 				EraseBack( size_t count = 1 );
		size_t 				CharacterCount() const;

		const FontFace* 	GetFont() const;
		void 				SetFont( const FontFace* font );

		void 				SetColor( const glm::vec4& color );
		const glm::vec4& 	GetColor() const;

		void 				SetFontSize( unsigned int fontsize );
		unsigned int 		GetFontSize() const;

		const glm::vec2		GetTextDimensions() const;

		// inherited from Renderable
		virtual void 		PostRenderCommands( Renderer* r, RenderCore* rc ) const;

	protected:
		void				RecalculateDimensions();

		const FontFace* 	mFont;
		std::string 		mText;
		glm::vec4			mColor;
		unsigned int 		mFontSize;
		glm::vec2 			mDims;
	};

    extern bool sDebugText;

} /* namespace Procyon */

#endif /* _TEXT_H */
