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
#ifndef _GL_TEXTURE_H
#define _GL_TEXTURE_H

#include "ProcyonGL.h"
#include "Texture.h"

namespace Procyon {

	class Image;

namespace GL {

	class GLTexture : public Texture
	{
	public:
						GLTexture();
						GLTexture( const std::string& filepath, int mipLevel = 0 );
						GLTexture( const IImage& img, int mipLevel = 0 );
		virtual			~GLTexture();

		virtual void	Bind() const;

		virtual void 	SetMinFilter( TextureFilterMode min );
		virtual void 	SetMagFilter( TextureFilterMode mag );
		virtual void	SetMinMagFilter( TextureFilterMode min, TextureFilterMode mag );

	protected:
		void 		SetData(const IImage& img, int mipLevel = 0);

		GLuint 		mTextureId;
		GLenum 		mTarget;
	};

} /* namespace GL */

} /* namespace Procyon */

#endif /* _GL_TEXTURE_H */
