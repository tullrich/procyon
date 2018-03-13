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
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "ProcyonCommon.h"
#include "Image.h"

namespace Procyon {

	enum TextureFilterMode
	{
		FILTER_LINEAR,
		FILTER_NEAREST,
		FILTER_NEAREST_MIPMAP_NEAREST,
		FILTER_NEAREST_MIPMAP_LINEAR,
		FILTER_LINEAR_MIPMAP_NEAREST,
		FILTER_LINEAR_MIPMAP_LINEAR
	};

	class Texture
	{
	public:
		virtual			~Texture() { }

		float			Width() const { return mDimensions.x; }
		float			Height() const { return mDimensions.y; }
		glm::vec2 		GetDimensions() const { return mDimensions; }

		virtual void 	Bind() const = 0;

		virtual void 	SetMinFilter( TextureFilterMode min ) = 0;
		virtual void 	SetMagFilter( TextureFilterMode mag ) = 0;
		virtual void	SetMinMagFilter( TextureFilterMode min, TextureFilterMode mag ) = 0;

		static Texture* Allocate( const std::string& filepath, int mipLevel = 0 );
		static Texture* Allocate( const IImage& img, int mipLevel = 0 );

	protected:
		glm::vec2 	mDimensions;
	};

} /* namespace Procyon */

#endif /* _TEXTURE_H */
