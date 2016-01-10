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

#include "Image.h"
#include "stb_image.h"

namespace Procyon {

	ImageBase::ImageBase()
		: mData( NULL )
		, mWidth( 0 )
		, mHeight( 0 )
		, mComponents( 0 )
	{
	}

	int ImageBase::Width() const
	{
		return mWidth;
	}

	int ImageBase::Height() const
	{
		return mHeight;
	}

	int ImageBase::Components() const
	{
		return mComponents;
	}

	const unsigned char* ImageBase::Data() const
	{
		return mData;
	}

	FileImage::FileImage( const std::string& filename)
	{
		mData = stbi_load(filename.c_str(), &mWidth, &mHeight, &mComponents, 0);

		if( !mData )
			throw std::runtime_error( "Unable to load image '" + filename + "'" );

	    PROCYON_DEBUG("Image", "Loaded '%s'. Width: %i Height: %i Components: %i."
	    	, filename.c_str(), mWidth, mHeight, mComponents );
	}

	FileImage::~FileImage()
	{
		stbi_image_free( mData );
	}


	MemoryImage::MemoryImage( int width, int height, int components )
	{
		mWidth = width;
		mHeight = height;
		mComponents = glm::clamp<int>( components, 1, 4 );
		mData = new unsigned char[ mWidth * mHeight * mComponents ]();
	}

	MemoryImage::MemoryImage( const IImage& in, int components )
	{
		mWidth 		= in.Width();
		mHeight 	= in.Height();
		mComponents = glm::clamp<int>( components, 1, 4 );
		mData 		= new unsigned char[ mWidth * mHeight * mComponents ]();

		const uint8_t* inData 	= in.Data();
		int inComponents 		= in.Components();
		for ( int x = 0; x < mWidth * mHeight; x++ )
		{
			mData[ x * mComponents + 0 ] = inData[ x * inComponents ];

			if ( mComponents >= 2 )
			{
				mData[ x * mComponents + 1 ]
					= ( inComponents >= 2 ) ? inData[ x * inComponents + 1 ] : 0;
			}
			if ( mComponents >= 3 )
			{
				mData[ x * mComponents + 2 ]
					= ( inComponents >= 3 ) ? inData[ x * inComponents + 2 ] : 0;
			}
			if ( mComponents >= 4 )
			{
				mData[ x * mComponents + 3 ]
					= ( inComponents >= 4 ) ? inData[ x * inComponents + 3 ] : 0;
			}
		}
	}

	MemoryImage::~MemoryImage()
	{
		delete[] mData;
	}

	unsigned char* MemoryImage::MutableData()
	{
		return mData;
	}

} /* namespace Procyon */
