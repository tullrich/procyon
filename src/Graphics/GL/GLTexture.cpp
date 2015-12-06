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
#include "GLTexture.h"
#include <stdexcept>

namespace Procyon {

namespace GL { 

	GLTexture::GLTexture()
		: mTextureId( -1 )
		, mTarget( GL_TEXTURE_2D )
	{
	    glGenTextures( 1, &mTextureId );
	}

	GLTexture::GLTexture( GLenum target, const std::string& filepath, int mipLevel /* = 0 */ )
		: mTextureId( -1 )
		, mTarget( GL_TEXTURE_2D )
	{
	    glGenTextures( 1, &mTextureId );

        FileImage img( filepath );
	    SetData( target, img, mipLevel );
	}

	GLTexture::GLTexture( GLenum target, const IImage& img, int mipLevel /* = 0 */ )
		: mTextureId( -1 )
		, mTarget( GL_TEXTURE_2D )
	{
	    glGenTextures( 1, &mTextureId );
	    SetData( target, img, mipLevel );
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures( 1, &mTextureId );
	}

	void GLTexture::Bind() const
	{
		glBindTexture( GL_TEXTURE_2D, mTextureId );
	}

	void GLTexture::SetData( GLenum target, const IImage& img, int mipLevel /* = 0 */ )
	{
		GLint format;
		switch( img.Components() )
		{
			case 1: format = GL_ALPHA; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
			default: throw std::runtime_error("Unsupported GL image format.");
		}

		glBindTexture( target, mTextureId );

	    glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTexImage2D( target, mipLevel, format, img.Width(), img.Height(), 0, format, GL_UNSIGNED_BYTE, img.Data() );
	    glGenerateMipmap( target );

	    mTarget = target;
	    mDimensions = glm::vec2( img.Width(), img.Height() );
	}

	int	GLTexture::Width() const
	{
		return mDimensions.x;
	}

	int	GLTexture::Height() const
	{
		return mDimensions.y;
	}

	glm::vec2 GLTexture::GetDimensions() const
	{
		return mDimensions;
	}

	void GLTexture::SetMinFilter( GLenum min )
	{
		glBindTexture( mTarget, mTextureId );
	    glTexParameteri( mTarget, GL_TEXTURE_MIN_FILTER, min );
	}

	void GLTexture::SetMagFilter( GLenum mag )
	{
		glBindTexture( mTarget, mTextureId );
	    glTexParameteri( mTarget, GL_TEXTURE_MAG_FILTER, mag );
	}

	void GLTexture::SetMinMagFilter( GLenum min, GLenum mag )
	{
		glBindTexture( mTarget, mTextureId );
	    glTexParameteri( mTarget, GL_TEXTURE_MIN_FILTER, min );
	    glTexParameteri( mTarget, GL_TEXTURE_MAG_FILTER, mag );
	}

} /* namespace GL */

} /* namespace Procyon */
