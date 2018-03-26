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
#include "Image.h"

namespace Procyon {

namespace GL {

	GLTexture::GLTexture()
		: mTextureId( -1 )
		, mTarget( GL_TEXTURE_2D )
	{
	    glGenTextures( 1, &mTextureId );
	}

	GLTexture::GLTexture( const std::string& filepath, int mipLevel /* = 0 */ )
		: mTextureId( -1 )
		, mTarget( GL_TEXTURE_2D )
	{
	    glGenTextures( 1, &mTextureId );

        FileImage img( filepath );
	    SetData( img, mipLevel );
	}

	GLTexture::GLTexture( const IImage& img, int mipLevel /* = 0 */ )
		: mTextureId( -1 )
		, mTarget( GL_TEXTURE_2D )
	{
	    glGenTextures( 1, &mTextureId );
	    SetData( img, mipLevel );
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures( 1, &mTextureId );
	}

	void GLTexture::Bind() const
	{
		glBindTexture( mTarget, mTextureId );
	}

	void GLTexture::SetData( const IImage& img, int mipLevel /* = 0 */ )
	{
		GLint format;
		switch( img.Components() )
		{
			case 1: format = GL_RED; break;
			case 2: format = GL_RG; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
			default: format = GL_RGBA; break; // unreachable
		}

        Bind();

	    glTexParameteri( mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT );
	    glTexParameteri( mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT );
	    glTexParameteri( mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	    glTexParameteri( mTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTexImage2D( mTarget, mipLevel, format, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, img.Data() );

	    mDimensions = glm::ivec2( img.GetWidth(), img.GetHeight() );
        glBindTexture( mTarget, 0 );
	}

	void GLTexture::SetMinFilter( TextureFilterMode min )
	{
		GLint filter;
		switch ( min )
		{
			case FILTER_LINEAR: filter = GL_LINEAR; break;
			case FILTER_NEAREST: filter = GL_NEAREST; break;
			case FILTER_NEAREST_MIPMAP_NEAREST: filter = GL_NEAREST_MIPMAP_NEAREST; break;
			case FILTER_NEAREST_MIPMAP_LINEAR: filter = GL_NEAREST_MIPMAP_LINEAR; break;
			case FILTER_LINEAR_MIPMAP_NEAREST: filter = GL_LINEAR_MIPMAP_NEAREST; break;
			case FILTER_LINEAR_MIPMAP_LINEAR: filter = GL_LINEAR_MIPMAP_LINEAR; break;
			default: filter = GL_LINEAR; break;
		}

		glBindTexture( mTarget, mTextureId );
	    glTexParameteri( mTarget, GL_TEXTURE_MIN_FILTER, filter );
	}

	void GLTexture::SetMagFilter( TextureFilterMode mag )
	{
		GLint filter;
		switch ( mag )
		{
			case FILTER_LINEAR: filter = GL_LINEAR; break;
			case FILTER_NEAREST: filter = GL_NEAREST; break;
			default: filter = GL_LINEAR; break;
		}

		glBindTexture( mTarget, mTextureId );
	    glTexParameteri( mTarget, GL_TEXTURE_MAG_FILTER, filter );
	}

	void GLTexture::SetMinMagFilter( TextureFilterMode min, TextureFilterMode mag )
	{
		SetMinFilter( min );
		SetMagFilter( mag );
	}

	void GLTexture::GenerateMipmap()
	{
		glBindTexture( mTarget, mTextureId );
		glGenerateMipmap( mTarget );
	}

}

	/*static*/ Texture* Texture::Allocate(const std::string& filepath, int mipLevel /* = 0 */)
	{
		return new GL::GLTexture(filepath, mipLevel);
	}

	/*static*/ Texture* Texture::Allocate(const IImage& img, int mipLevel /* = 0 */)
	{
		return new GL::GLTexture(img, mipLevel);
	}

} /* namespace Procyon */
