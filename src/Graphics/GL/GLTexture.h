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

#include "ProcyonGL.h"
#include "Image.h"

namespace Procyon {

namespace GL { 

	class GLTexture
	{
	public:
					GLTexture();
					GLTexture( GLenum target, const std::string& filepath, int mipLevel = 0 );
					GLTexture( GLenum target, const IImage& img, int mipLevel = 0 );
					~GLTexture();

		void		Bind() const;
		void 		SetData( GLenum target, const IImage& img, int mipLevel = 0 );

		int			Width() const;
		int			Height() const;
		glm::vec2 	GetDimensions() const;

		void 		SetMinFilter( GLenum min );
		void 		SetMagFilter( GLenum mag );
		void		SetMinMagFilter( GLenum min, GLenum mag );

		GLuint 		mTextureId;

	protected:
		GLenum 		mTarget;
		glm::vec2 	mDimensions;
	};

} /* namespace GL */

} /* namespace Procyon */

#endif /* _TEXTURE_H */